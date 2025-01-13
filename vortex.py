import socket
from threading import Thread
from cryptography.hazmat.primitives.ciphers import Cipher, algorithms, modes
from cryptography.hazmat.backends import default_backend
from cryptography.hazmat.primitives import padding
from cryptography.fernet import Fernet
from PyQt5.QtWidgets import QApplication, QWidget, QVBoxLayout, QHBoxLayout, QPushButton, QLineEdit, QTextEdit, QDialog, QGridLayout
import os
import random
import smtplib
from email.mime.text import MIMEText
import time
from supabase import create_client, Client

# Server's IP address and port
SERVER_HOST = "10.0.1.35"  # Update if the server is on a different machine
SERVER_PORT = 9999  # Updated port
separator_token = "┃"  # Using "┃" as the separator token

# The AES-GCM key directly embedded (32 bytes for AES-256)
key = bytes.fromhex("a6851b9258192de78a4539464f58cb85b7d2ddd88ca7f23bc86075a416059b3f")

# Fernet key
fernet_key = b"ZILOyzMczOcdCWLTYqVcVMU4lCnQfLo0aowZBwMryX4="
fernet = Fernet(fernet_key)

# Initialize Supabase client
url = "https://uqwylxwxnxasfrrxmcsp.supabase.co"
api_key = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InVxd3lseHd4bnhhc2ZycnhtY3NwIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MzY0OTYzMTIsImV4cCI6MjA1MjA3MjMxMn0.63rctEDbwaOhRu8D2yM2z3XFoklTfcrN7VQzKNfJozY"
supabase: Client = create_client(url, api_key)


# Function to encrypt messages using AES-GCM and Fernet
def encrypt_message(message):
    """Encrypt message using AES-GCM and Fernet."""
    # First, encrypt the message with Fernet
    fernet_encrypted_message = fernet.encrypt(message.encode())

    # Then, encrypt the Fernet encrypted message using AES-GCM
    nonce = os.urandom(12)
    padder = padding.PKCS7(128).padder()
    padded_data = padder.update(fernet_encrypted_message) + padder.finalize()

    cipher = Cipher(algorithms.AES(key), modes.GCM(nonce), backend=default_backend())
    encryptor = cipher.encryptor()

    ciphertext = encryptor.update(padded_data) + encryptor.finalize()

    # Return nonce, tag, and ciphertext
    return nonce + encryptor.tag + ciphertext


def decrypt_message(encrypted_message):
    """Decrypt message using AES-GCM and Fernet."""
    nonce = encrypted_message[:12]
    tag = encrypted_message[12:28]  # 16 bytes for GCM tag
    ciphertext = encrypted_message[28:]

    # Decrypt the message using AES-GCM
    cipher = Cipher(algorithms.AES(key), modes.GCM(nonce, tag), backend=default_backend())
    decryptor = cipher.decryptor()
    padded_data = decryptor.update(ciphertext) + decryptor.finalize()

    # Remove padding
    unpadder = padding.PKCS7(128).unpadder()
    unpadded_data = unpadder.update(padded_data) + unpadder.finalize()

    # Decrypt the message with Fernet
    return fernet.decrypt(unpadded_data).decode()


# Initialize TCP socket
s = socket.socket()
print(f"[*] Connecting to {SERVER_HOST}:{SERVER_PORT}...")

s.connect((SERVER_HOST, SERVER_PORT))
print("[+] Connected.")

# Ask for email for OTP verification
email = input("ENTER YOUR MAIL: ")

# Generate a random OTP
otp = str(random.randint(100000, 999999))


# Function to send OTP to email
def send_otp_email(email, otp):
    try:
        sender_email = "vortex@infopeklo.cz"
        receiver_email = email
        subject = "ZDE JE VÁŠ KOD pro Vbox CHAT:"
        body = f"NIKOMU TO NEŘÍKEJTE: {otp}"

        msg = MIMEText(body)
        msg['From'] = sender_email
        msg['To'] = receiver_email
        msg['Subject'] = subject

        with smtplib.SMTP('smtp.seznam.cz', 587) as server:
            server.starttls()
            server.login(sender_email, "Polik789@")
            server.sendmail(sender_email, receiver_email, msg.as_string())
            print(f"[+] OTP sent to {email}")
    except Exception as e:
        print(f"[!] Error sending OTP: {e}")


send_otp_email(email, otp)
entered_otp = input("ENTER VERIFICATION CODE: ")

if entered_otp == otp:
    print("[+] OTP verified successfully.")
else:
    print("[!] Invalid OTP.")
    s.close()
    exit()

username = input("Enter your username: ")
password = input("Enter your password: ")
s.send(f"{username}{separator_token}{password}".encode())

auth_response = s.recv(1024).decode()

if auth_response != "Authentication successful.":
    print(f"[!] Authentication failed: {auth_response}")
    s.close()
else:
    print("[+] Authentication successful.")

    # Initialize chat history as a list (in-memory storage)
    chat_history = []


    # Function to fetch previous messages from Supabase
    def fetch_previous_messages():
        try:
            # Fetch messages from Supabase
            response = supabase.table('desktop').select('user, message').execute()

            if response.status_code == 200:
                # Successfully fetched messages
                messages = response.data
                for message in messages:
                    # Add each message to the chat display
                    chat_display.append(f"<font color='blue'>[{message['user']}]: {message['message']}</font>")
            else:
                print("[!] Error fetching messages from Supabase")
        except Exception as e:
            print(f"[!] Error: {e}")


    # Fetch and display previous messages
    fetch_previous_messages()


    # Emoji Picker Dialog
    class EmojiPickerDialog(QDialog):
        def __init__(self):
            super().__init__()
            self.setWindowTitle("Emoji Picker")
            self.setGeometry(100, 100, 250, 250)

            layout = QVBoxLayout()

            self.emoji_grid = QGridLayout()
            self.emojis = [
                "😊", "😂", "❤️", "👍", "😢", "😎", "😍", "😇", "🤔", "🥺",
                "💀", "💩", "😡", "🤪", "🎉", "💔", "🔥", "😜", "🙃", "💖"
            ]

            row, col = 0, 0
            for emoji in self.emojis:
                button = QPushButton(emoji)
                button.clicked.connect(self.insert_emoji)
                self.emoji_grid.addWidget(button, row, col)
                col += 1
                if col == 5:  # Adjust the number of emojis per row
                    col = 0
                    row += 1

            layout.addLayout(self.emoji_grid)
            self.setLayout(layout)

        def insert_emoji(self):
            button = self.sender()
            emoji = button.text()
            message_input.setText(message_input.text() + emoji)
            self.accept()


    # Main window creation
    app = QApplication([])

    window = QWidget()
    window.setWindowTitle("Vortex")
    window.setGeometry(100, 100, 500, 400)

    main_layout = QVBoxLayout()
    message_layout = QHBoxLayout()

    chat_display = QTextEdit()
    chat_display.setReadOnly(True)
    chat_display.setStyleSheet("background-color: #1e1e1e; color: white;")
    main_layout.addWidget(chat_display)

    message_input = QLineEdit()
    message_input.setPlaceholderText("Type your message...")
    message_input.setStyleSheet("background-color: #333333; color: white; border: 1px solid #555555;")
    message_layout.addWidget(message_input)

    send_button = QPushButton("➤")
    send_button.setStyleSheet("background-color: #4CAF50; color: white; border-radius: 5px; padding: 5px 15px;")
    message_layout.addWidget(send_button)

    emoji_button = QPushButton("🙂")
    emoji_button.setStyleSheet("background-color: #f1c40f; color: white; border-radius: 5px; padding: 5px 15px;")
    message_layout.addWidget(emoji_button)

    main_layout.addLayout(message_layout)
    window.setLayout(main_layout)


    def send_message():
        to_send = message_input.text()
        if to_send.lower() == 'q':
            window.close()
            s.close()
            return

        message = f"{username}{separator_token}{to_send}"

        encrypted_message = encrypt_message(message)

        chat_history.append(f"[{username}]: {to_send}")  # Add message to history

        chat_display.append(f"<font color='green'>[{username}]: {to_send}</font>")
        s.send(encrypted_message)
        message_input.clear()


    send_button.clicked.connect(send_message)


    def open_emoji_picker():
        dialog = EmojiPickerDialog()
        dialog.exec_()


    emoji_button.clicked.connect(open_emoji_picker)


    def listen_for_messages():
        while True:
            try:
                encrypted_message = s.recv(1024)
                message = decrypt_message(encrypted_message)

                chat_display.append(f"<font color='green'>[další]: {message}</font>")

            except Exception as e:
                break


    t = Thread(target=listen_for_messages)
    t.daemon = True
    t.start()

    window.show()
    app.exec_()

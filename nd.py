from supabase import create_client, Client
import os

# Inicializace Supabase klienta
url = "https://uqwylxwxnxasfrrxmcsp.supabase.co"  # Nahraď vlastním URL
key = "eyJhbGciOiJIUzI1NiIsInR5cCI6IkpXVCJ9.eyJpc3MiOiJzdXBhYmFzZSIsInJlZiI6InVxd3lseHd4bnhhc2ZycnhtY3NwIiwicm9sZSI6ImFub24iLCJpYXQiOjE3MzY0OTYzMTIsImV4cCI6MjA1MjA3MjMxMn0.63rctEDbwaOhRu8D2yM2z3XFoklTfcrN7VQzKNfJozY"  # Nahraď vlastním veřejným klíčem
supabase: Client = create_client(url, key)

def upload_background(image_path, user_id, chat_id):
    # Otevření souboru a čtení jeho obsahu
    with open(image_path, "rb") as file:
        file_data = file.read()

    # Nahrání souboru do Supabase Storage
    storage = supabase.storage()
    bucket_name = "chats"  # Tento bucket by měl být vytvořen v Supabase
    file_name = f"{user_id}_{chat_id}_background.jpg"

    # Nahrání souboru
    storage.from_(bucket_name).upload(file_name, file_data)

    # Získání veřejného URL odkazu
    public_url = storage.from_(bucket_name).get_public_url(file_name)
    
    # Uložení URL do databáze (tabulka chats)
    response = supabase.table("chats").upsert({
        "user_id": user_id,
        "chat_id": chat_id,
        "background_url": public_url["publicURL"]
    }).execute()

    return public_url["publicURL"]

# Příklad použití
image_path = "C:\devil\vortex.png"  # Cesta k obrázku
user_id = "user-uuid"  # UUID uživatele
chat_id = "chat-uuid"  # UUID chatu
background_url = upload_background(image_path, user_id, chat_id)
print(f"Pozadí bylo úspěšně nahráno! URL: {background_url}")

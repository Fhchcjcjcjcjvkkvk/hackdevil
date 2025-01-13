import tkinter as tk
from PIL import Image, ImageTk
import requests
from io import BytesIO

# Create the main application window
root = tk.Tk()
root.title("App with Background Image")

# Set the size of the window
root.geometry("800x600")

# Load the image from the URL
image_url = "https://github.com/Fhchcjcjcjcjvkkvk/hackdevil/blob/main/vortex.png?raw=true"
response = requests.get(image_url)
img_data = BytesIO(response.content)

# Open the image using PIL
background_image = Image.open(img_data)
background_image = background_image.resize((800, 600), Image.ANTIALIAS)  # Resize to fit window

# Convert to PhotoImage format suitable for Tkinter
background_photo = ImageTk.PhotoImage(background_image)

# Create a canvas to display the background image
canvas = tk.Canvas(root, width=800, height=600)
canvas.pack(fill="both", expand=True)

# Display the background image on the canvas
canvas.create_image(0, 0, image=background_photo, anchor="nw")

# Example of adding a label on top of the background
label = tk.Label(root, text="Hello, Welcome to the App!", font=("Helvetica", 24), bg="white", fg="black")
label.place(relx=0.5, rely=0.1, anchor="center")

# Run the app
root.mainloop()

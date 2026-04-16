import tkinter as tk
from tkinter import messagebox
from api_request import RequestApi

class LoginApp:
    def __init__(self, root):
        self.root = root
        self.root.title("Login")
        self.root.geometry("300x150")

        self.label_user = tk.Label(root, text="Username:")
        self.label_user.pack()

        self.entry_user = tk.Entry(root)
        self.entry_user.pack()

        self.label_password = tk.Label(root, text="Password:")
        self.label_password.pack()

        self.entry_password = tk.Entry(root, show="*")
        self.entry_password.pack()

        self.login_button = tk.Button(root, text="Login", command=self.login)
        self.login_button.pack()

    def call_back(self):
        self.root.destroy()

    def login(self):
        username = self.entry_user.get()
        password = self.entry_password.get()
        if username == "1" and password == "1":
            self.on_login_success()  
            self.root.withdraw()

        else:
            messagebox.showerror("Login Failed", "Invalid username or password. Please try again.")
     
    def on_login_success(self):
        return RequestApi(self.root, self.call_back)
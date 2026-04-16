from api_login import LoginApp
import tkinter as tk

if __name__ == "__main__":
    root = tk.Tk()
    root.title("VINES-MES PROGRAM")
    root.geometry("400x200")
    root.eval('tk::PlaceWindow . center')
    request_table = LoginApp(root)
    root.mainloop()

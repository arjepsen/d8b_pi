import tkinter as tk
import tkinter.font as tkFont

# Initialize the display content array (2 rows by 40 columns)
display_content = [[" " for _ in range(40)] for _ in range(2)]

# Mapping of position commands to grid positions
position_map = {
    "80u": (0, 0), "81u": (0, 1), "82u": (0, 2), "83u": (0, 3), "84u": (0, 4), "85u": (0, 5), "86u": (0, 6), "87u": (0, 7),
    "88u": (0, 8), "89u": (0, 9), "8Au": (0, 10), "8Bu": (0, 11), "8Cu": (0, 12), "8Du": (0, 13), "8Eu": (0, 14), "8Fu": (0, 15),
    "90u": (0, 16), "91u": (0, 17), "92u": (0, 18), "93u": (0, 19), "94u": (0, 20), "95u": (0, 21), "96u": (0, 22), "97u": (0, 23),
    "98u": (0, 24), "99u": (0, 25), "9Au": (0, 26), "9Bu": (0, 27), "9Cu": (0, 28), "9Du": (0, 29), "9Eu": (0, 30), "9Fu": (0, 31),
    "A0u": (0, 32), "A1u": (0, 33), "A2u": (0, 34), "A3u": (0, 35), "A4u": (0, 36), "A5u": (0, 37), "A6u": (0, 38), "A7u": (0, 39),

    "C0u": (1, 0), "C1u": (1, 1), "C2u": (1, 2), "C3u": (1, 3), "C4u": (1, 4), "C5u": (1, 5), "C6u": (1, 6), "C7u": (1, 7),
    "C8u": (1, 8), "C9u": (1, 9), "CAu": (1, 10), "CBu": (1, 11), "CCu": (1, 12), "CDu": (1, 13), "CEu": (1, 14), "CFu": (1, 15),
    "B0u": (1, 8), "B1u": (1, 9), "B2u": (1, 10), "B3u": (1, 11), "B4u": (1, 12), "B5u": (1, 13), "B6u": (1, 14), "B7u": (1, 15),
    "D0u": (1, 16), "D1u": (1, 17), "D2u": (1, 18), "D3u": (1, 19), "D4u": (1, 20), "D5u": (1, 21), "D6u": (1, 22), "D7u": (1, 23),
    "D8u": (1, 24), "D9u": (1, 25), "DAu": (1, 26), "DBu": (1, 27), "DCu": (1, 28), "DDu": (1, 29), "DEu": (1, 30), "DFu": (1, 31),
    "E0u": (1, 32), "E1u": (1, 33), "E2u": (1, 34), "E3u": (1, 35), "E4u": (1, 36), "E5u": (1, 37), "E6u": (1, 38), "E7u": (1, 39),
}

reverse_position_map = {v: k for k, v in position_map.items()}  # Reverse the position map for easy lookup

def parse_input(input_text):
    """Parses the input text and updates the display_content array."""
    global display_content
    display_content = [[" " for _ in range(40)] for _ in range(2)]

    current_row, current_col = 0, 0

    i = 0
    length = len(input_text)

    while i < length:
        if input_text[i] == 'u':
            if i >= 2 and input_text[i-2:i].isalnum():
                hex_code = input_text[i-2:i]
                if hex_code + "u" in position_map:
                    current_row, current_col = position_map[hex_code + "u"]
            i += 1
        elif input_text[i] == 'v':
            if i >= 2 and input_text[i-2:i].isalnum():
                hex_code = input_text[i-2:i]
                try:
                    char = chr(int(hex_code, 16))
                    if current_col < 40:
                        display_content[current_row][current_col] = char
                        current_col += 1
                except ValueError:
                    pass
            i += 1
        else:
            i += 1

def update_display():
    """Updates the visual display based on the display_content array."""
    for row in range(2):
        for col in range(40):
            labels[row][col].config(text=display_content[row][col])

def on_update_display_button():
    """Handles the update display button click."""
    input_text = text_input.get("1.0", tk.END).strip()
    parse_input(input_text)
    update_display()

def enforce_single_character(event, text_var, entry):
    """Ensures that only one character can be entered in an Entry widget."""
    value = text_var.get()
    if len(value) > 1:
        text_var.set(value[-1])
    # If the entered character is a space, change background color
    if value == " ":
        entry.config(bg="blue")
    else:
        entry.config(bg="white")

def generate_command_string():
    """Generates the command string based on the content of the Entry widgets."""
    command_string = ""
    last_position_written = None

    for row in range(2):
        for col in range(40):
            char = entry_grid[row][col].get()
            if char:  # Handle spaces and non-empty characters
                if last_position_written is None or last_position_written != (row, col-1):
                    command_string += reverse_position_map[(row, col)]

                if char == " ":
                    command_string += "20v"
                else:
                    command_string += f"{ord(char):02X}v"

                last_position_written = (row, col)

    return command_string

def on_generate_command_button():
    """Handles the generate command button click."""
    command_string = generate_command_string()
    result_text.config(state=tk.NORMAL)
    result_text.delete(1.0, tk.END)
    result_text.insert(tk.END, command_string)
    result_text.config(state=tk.DISABLED)

def on_clear_fields_button():
    """Clears all the fields in the grid."""
    for row in range(2):
        for col in range(40):
            entry_grid[row][col].delete(0, tk.END)
            entry_grid[row][col].config(bg="white")  # Reset background color

root = tk.Tk()
root.title("Hex Display Simulator")

# Set a fixed-width font for proper alignment
font = tkFont.Font(family="Courier", size=10)

# Create a Frame for the text input field and scrollbar
input_frame = tk.Frame(root)
input_frame.grid(row=0, column=0, columnspan=40, pady=10)

# Create the Text widget for multiline input with word wrapping
text_input = tk.Text(input_frame, wrap=tk.WORD, width=80, height=5, font=font)
text_input.pack(side=tk.LEFT, fill=tk.BOTH, expand=True)

# Add a vertical scrollbar to the Text widget
scrollbar = tk.Scrollbar(input_frame, command=text_input.yview)
scrollbar.pack(side=tk.RIGHT, fill=tk.Y)
text_input.config(yscrollcommand=scrollbar.set)

# Button to update display
button = tk.Button(root, text="Update Display", command=on_update_display_button)
button.grid(row=1, column=0, columnspan=40, pady=(0, 10))

# Create a single Frame to contain the whole display grid
display_frame = tk.Frame(root, bg="#404040")
display_frame.grid(row=2, column=0, columnspan=40)

# Create the display grid inside the frame
labels = [[None for _ in range(40)] for _ in range(2)]
for row in range(2):
    for col in range(40):
        label = tk.Label(display_frame, text=" ", width=1, height=1, padx=2, pady=2, font=font, bg="black", fg="cyan", relief="flat")
        label.grid(row=row, column=col, padx=1, pady=1)
        labels[row][col] = label

# Add a separator line between the display and the input grid
separator_frame = tk.Frame(root, height=2, bd=1, relief=tk.SUNKEN)
separator_frame.grid(row=3, column=0, columnspan=40, pady=10, sticky="we")

# --- New Section: Enter Chars into the Grid and Generate Command String ---

# Create a Frame for the reverse input grid
reverse_input_frame = tk.Frame(root)
reverse_input_frame.grid(row=4, column=0, columnspan=40, pady=10)

# Create the grid of Entry widgets for direct input
entry_grid = [[None for _ in range(40)] for _ in range(2)]
for row in range(2):
    for col in range(40):
        text_var = tk.StringVar()
        entry = tk.Entry(reverse_input_frame, width=2, font=font, textvariable=text_var, justify="center")
        entry.grid(row=row, column=col, padx=1, pady=1)
        entry.bind("<KeyRelease>", lambda event, var=text_var, ent=entry: enforce_single_character(event, var, ent))
        entry_grid[row][col] = entry

# Button to generate the command string
generate_button = tk.Button(root, text="Generate Command String", command=on_generate_command_button)
generate_button.grid(row=5, column=0, columnspan=40, pady=(0, 10))

# Button to clear all fields in the grid
clear_button = tk.Button(root, text="Clear Fields", command=on_clear_fields_button)
clear_button.grid(row=6, column=0, columnspan=40, pady=(0, 10))

# Text widget to display the generated command string
result_text = tk.Text(root, height=3, width=80, font=font, state=tk.DISABLED, wrap=tk.WORD)
result_text.grid(row=7, column=0, columnspan=40, pady=10)

root.mainloop()

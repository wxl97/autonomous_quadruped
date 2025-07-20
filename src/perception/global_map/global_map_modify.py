# import cv2
# import numpy as np
# import tkinter as tk
# from tkinter import filedialog
# import yaml
# from PIL import Image, ImageTk

# class MapEditor:
#     def __init__(self, root):
#         self.root = root
#         self.root.title("Map Editor")

#         self.canvas = tk.Canvas(root, cursor="cross")
#         self.canvas.pack(fill=tk.BOTH, expand=True)

#         self.rect = None
#         self.start_x = None
#         self.start_y = None
#         self.rect_id = None
#         self.image_path = None
#         self.yaml_path = None

#         self.canvas.bind("<Button-1>", self.on_button_press)
#         self.canvas.bind("<B1-Motion>", self.on_move_press)
#         self.canvas.bind("<ButtonRelease-1>", self.on_button_release)

#         menu = tk.Menu(root)
#         root.config(menu=menu)
#         file_menu = tk.Menu(menu)
#         menu.add_cascade(label="File", menu=file_menu)
#         file_menu.add_command(label="Open Map", command=self.open_file)
#         file_menu.add_command(label="Save Map", command=self.save_file)
#         file_menu.add_command(label="Save and Exit", command=self.save_and_exit)
#         file_menu.add_separator()
#         file_menu.add_command(label="Exit", command=root.quit)

#     def open_file(self):
#         self.image_path = filedialog.askopenfilename(filetypes=[("PGM files", "*.pgm")])
#         self.yaml_path = filedialog.askopenfilename(filetypes=[("YAML files", "*.yaml")])

#         if self.image_path:
#             self.image = cv2.imread(self.image_path, cv2.IMREAD_GRAYSCALE)
#             self.display_image()

#     def save_file(self):
#         if self.rect:
#             x1, y1, x2, y2 = self.canvas.coords(self.rect_id)
#             self.modify_map_with_box(int(x1), int(y1), int(x2), int(y2))

#     def save_and_exit(self):
#         self.save_file()
#         self.root.quit()

#     def display_image(self):
#         self.tk_image = ImageTk.PhotoImage(image=Image.fromarray(self.image))
#         self.canvas.create_image(0, 0, anchor=tk.NW, image=self.tk_image)
#         self.canvas.config(scrollregion=self.canvas.bbox(tk.ALL))

#     def on_button_press(self, event):
#         self.start_x = event.x
#         self.start_y = event.y
#         self.rect = None

#     def on_move_press(self, event):
#         cur_x, cur_y = (event.x, event.y)
#         if self.rect:
#             self.canvas.coords(self.rect_id, self.start_x, self.start_y, cur_x, cur_y)
#         else:
#             self.rect_id = self.canvas.create_rectangle(self.start_x, self.start_y, cur_x, cur_y, outline="red")

#     def on_button_release(self, event):
#         pass

#     def modify_map_with_box(self, x1, y1, x2, y2):
#         try:
#             self.image[y1:y2, x1:x2] = 255  # 将框内的所有像素值设为255
#             modified_image_path = "global_modified_map.pgm"
#             modified_image = Image.fromarray(self.image)
#             modified_image.save(modified_image_path)

#             with open(self.yaml_path, 'r') as file:
#                 yaml_data = yaml.safe_load(file)

#             yaml_data['image'] = modified_image_path

#             modified_yaml_path = "global_modified_map.yaml"
#             with open(modified_yaml_path, 'w') as file:
#                 yaml.dump(yaml_data, file, default_flow_style=False)

#             print("Map saved successfully!")
#             print(f"Modified map saved as: {modified_image_path}")
#             print(f"Modified YAML saved as: {modified_yaml_path}")

#         except Exception as e:
#             print(f"An error occurred: {e}")

# if __name__ == "__main__":
#     root = tk.Tk()
#     app = MapEditor(root)
#     root.mainloop()


from PIL import Image

# read image
pgm_image = Image.open('src/perception/global_map/modified_global_map.pgm')

# convert image to RGB
pgm_image = pgm_image.convert('RGB')

# get image size
pixels = pgm_image.load()

# set running way to black
for i in range(90, 135):
    for j in range(232,235):
        pixels[i, j] = (0, 0, 0)
        
# set running way to black
for i in range(93, 135):
    for j in range(236,240):
        pixels[i, j] = (255, 255, 255)
        
# set running way to black
for i in range(90, 93):
    for j in range(237,275):
        pixels[i, j] = (0, 0, 0)

# set running way to black
pgm_image.save('modified_global_map_1.pgm')

from tkinter import *
from math import sqrt
from numpy import arange


def get_graph_points(a_value):
    points = list()

    if a_value == 0.0:
        points.append([0, 0, 0])
        return points

    number_of_iterations = 100  # smaller -> faster -> less accurate
    step = abs(a_value) / number_of_iterations

    if a_value < 0:
        points.append([a_value, 0, 0])

    for x in arange(-1 * abs(a_value), 0, step):
        if a_value + x == 0.0:
            continue
        first_root = sqrt(x * x * (a_value - x) / (a_value + x))
        second_root = -1 * sqrt(x * x * (a_value - x) / (a_value + x))
        points.append([x, first_root, second_root])

    for x in arange(0, abs(a_value), step):
        if a_value + x == 0.0:
            continue
        first_root = sqrt(x * x * (a_value - x) / (a_value + x))
        second_root = -1 * sqrt(x * x * (a_value - x) / (a_value + x))
        points.append([x, first_root, second_root])

    if a_value > 0:
        points.append([a_value, 0, 0])

    return points


class Window:
    def __init__(self, name='lab_name', variables=None):
        self.root = Tk()
        self.root.title(name)
        self.root.geometry('700x700')
        self.root.minsize(500, 500)

        self.canvas = Canvas(self.root, bg='white')
        self.canvas.pack(fill=BOTH, pady=5, padx=5, expand=1)
        self.canvas.bind('<ButtonPress-1>', lambda event: self.canvas.scan_mark(event.x, event.y))
        self.canvas.bind("<B1-Motion>", lambda event: self.canvas.scan_dragto(event.x, event.y, gain=1))
        self.canvas.bind("<MouseWheel>", self.zoom)

        if variables is not None:
            self.button = Button(self.root, text='Draw', font='Times 12', command=self.draw_graph)
            self.button.pack(side=RIGHT, pady=15, padx=5, ipadx=14, ipady=7, anchor=SE)

            self.text = Entry(self.root, justify='center', font='Times 15', width=7)
            self.text.pack(side=RIGHT, padx=5, ipadx=0.1, ipady=10, anchor=SE, pady=15)

            self.label_eq = Label(text='=', font='Times 18')
            self.label_eq.pack(side=RIGHT, anchor=SE, pady=19)

            self.label = Label(text=variables, font='Times 20')
            self.label.pack(side=RIGHT, anchor=SE, pady=20)

            self.equation = Label(text='y^2 = x^2 * (a - x)/(a + x)', font='Times 20')
            self.equation.pack(side=RIGHT, anchor=S, pady=20, expand=1)

    def zoom(self, event):
        factor = 1.001 ** event.delta
        self.canvas.scale(ALL, event.x, event.y, factor, factor)

    def draw_current_graph(self, points_list, a_value):
        w = self.canvas.winfo_width()
        h = self.canvas.winfo_height()

        for i in range(0, len(points_list) - 1):
            self.canvas.create_line(points_list[i][0] * 25 + w // 2,
                                    points_list[i][1] * 25 + h // 2,
                                    points_list[i + 1][0] * 25 + w // 2,
                                    points_list[i + 1][1] * 25 + h // 2,
                                    width=1.5)

            self.canvas.create_line(points_list[i][0] * 25 + w // 2,
                                    points_list[i][2] * 25 + h // 2,
                                    points_list[i + 1][0] * 25 + w // 2,
                                    points_list[i + 1][2] * 25 + h // 2,
                                    width=1.5)

        if len(points_list) == 1:
            self.canvas.create_oval(points_list[0][0] * 25 + w // 2,
                                    points_list[0][1] * 25 + h // 2,
                                    points_list[0][0] * 25 + w // 2,
                                    points_list[0][2] * 25 + h // 2,
                                    width=5)
        elif a_value > 0:
            self.canvas.create_line(points_list[len(points_list) - 1][0] * 25 + w // 2,
                                    points_list[len(points_list) - 1][1] * 25 + h // 2,
                                    points_list[len(points_list) - 1][0] * 25 + w // 2,
                                    points_list[len(points_list) - 1][2] * 25 + h // 2,
                                    width=2)
        else:
            self.canvas.create_line(points_list[0][0] * 25 + w // 2,
                                    points_list[0][1] * 25 + h // 2,
                                    points_list[0][0] * 25 + w // 2,
                                    points_list[0][2] * 25 + h // 2,
                                    width=2)

    def draw_cords(self):
        c_w = self.canvas.winfo_width() // 2
        c_h = self.canvas.winfo_height() // 2
        self.canvas.create_line(c_w, c_h, c_w, 2000, dash=(25, 25))
        self.canvas.create_line(c_w, c_h, 2500, c_h, dash=(25, 25))
        self.canvas.create_line(c_w, c_h, c_w, -1500, dash=(25, 25))
        self.canvas.create_line(c_w, c_h, -1500, c_h, dash=(25, 25))

    def draw_graph(self):
        try:
            val = float(self.text.get())
            self.canvas.delete(ALL)
            self.draw_cords()
            self.draw_current_graph(get_graph_points(val), val)
        except:
            self.text.insert('0', '0')

    def show_window(self):
        self.root.mainloop()

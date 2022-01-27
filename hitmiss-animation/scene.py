from manim import *
import numpy as np
import cv2
import manimpango

# small_blob data
image = np.array([
    [0, 0, 0, 0, 0, 0, 0, 0],
    [0, 0, 0, 0, 0, 0, 1, 0],
    [0, 1, 1, 1, 1, 1, 1, 0],
    [0, 1, 1, 1, 1, 1, 0, 0],
    [0, 1, 0, 0, 0, 0, 0, 0],
    [0, 1, 1, 1, 1, 1, 1, 0],
    [0, 0, 1, 1, 1, 1, 1, 0],
    [0, 0, 0, 0, 0, 0, 0, 0]
], dtype="int8")

kernel = np.array([
    [0, 0, 0],
    [1, 1, 1],
    [-1, 1, -1]
], dtype="int8")


def color_cells(table: MathTable):
    for i, row in enumerate(image, 1):
        for j, pixel in enumerate(row, 1):
            if (pixel == 1):
                table.add_highlighted_cell(
                    (i, j), color=color.WHITE, fill_opacity=1.0)
                table.get_entries((i, j)).set_color(color.BLACK)


class CreateTable(Scene):
    def construct(self):
        scene_title = Text(
            "Hit-Or-Miss Transform Visualized", font="Noto Sans")
        t0 = IntegerTable(image, include_outer_lines=True)

        color_cells(t0)
        t0.scale(0.4)
        t0.shift(LEFT*4)
        cell_width = t0.get_cell((1, 1)).width
        cell_height = t0.get_cell((1, 1)).height

        t0_title = Text("Image", font_size=15, font="Noto Sans")
        t0_title.next_to(t0, UP)

        kernel_table = IntegerTable(kernel, include_outer_lines=True)
        kernel_table.set_color(BLUE_D)

        kernel_table.scale(0.4)

        kernel_title = Text("Kernel", font_size=15, font="Noto Sans")
        kernel_title.next_to(kernel_table, UP)

        rect = Rectangle(color=BLUE_D, height=kernel_table.height,
                         width=cell_width*3)
        rect_2 = Rectangle(color=BLUE_D, height=cell_height, width=cell_width)

        t1 = IntegerTable(np.zeros(image.shape, dtype="uint8"),
                          include_outer_lines=True)
        t1.scale(0.4)
        t1.shift(RIGHT*4)

        t1_title = Text("Hit-Or-Miss Output", font_size=15, font="Noto Sans")
        t1_title.next_to(t1, UP)

        self.add(scene_title)
        self.play(scene_title.animate.move_to(UP*3))

        self.add(t0)
        self.play(Create(t0))
        self.add(t0_title)

        self.add(t1)
        self.play(Create(t1))
        self.add(t1_title)

        self.add(kernel_table)
        self.play(Create(kernel_table), run_time=0.8)
        self.add(kernel_title)

        self.add(rect)
        self.play(rect.animate.align_to(t0, UL), run_time=0.8)
        self.add(rect_2)
        self.play(rect_2.animate.align_to(t1, UL).shift(
            RIGHT*cell_width+DOWN*cell_height), run_time=0.8)

        for i, row in enumerate(image[1:-1], 2):
            for j, pixel in enumerate(row[1:-1], 2):
                sub_image = image[i-2:i+1, j-2:j+1]
                #print((i, j))
                #print(sub_image)
                #print(kernel)

                match = True
                for r in range(0, len(kernel)):
                    for c in range(0, len(kernel[r])):
                        if kernel[r, c] != -1 and sub_image[r, c] != kernel[r, c]:
                            match = False
                            break
                #print(match)
                if match:
                    t1.add_highlighted_cell(
                        (i, j), color=color.WHITE, fill_opacity=1.0)
                    e = t1.get_entries((i, j))
                    e.set_color(color.BLACK)
                    e.set_value(1)
                if j < len(row)-1:
                    self.play(rect.animate.shift(
                        RIGHT*cell_width), rect_2.animate.shift(
                        RIGHT*cell_width), run_time=0.8)

            if i < len(image)-1:
                self.play(rect.animate.shift(
                    DOWN*cell_height).align_to(t0, LEFT), rect_2.animate.align_to(t1, LEFT).shift(
                    DOWN*cell_height+RIGHT*cell_width), run_time=0.8)
            else:
                self.remove(rect, rect_2)

        self.wait(1)

from manim import *
from matplotlib.pyplot import table
import numpy as np
import cv2
import manimpango

SCALE_VALUE = 0.4
RUN_TIME = 0.8

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

kernel_temp = np.array([
    [0, 0, 0],
    [1, 1, 1],
    [0, 1, 0]
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
        t0.scale(SCALE_VALUE)
        t0.shift(LEFT*4)
        cell_width = t0.get_cell((1, 1)).width
        cell_height = t0.get_cell((1, 1)).height

        t0_title = Text("Image", font_size=15, font="Noto Sans")
        t0_title.next_to(t0, UP)

        kernel_table = IntegerTable(kernel, include_outer_lines=True)
        kernel_table.set_color(BLUE_D)

        kernel_table.scale(SCALE_VALUE)

        moving_kernel = IntegerTable(
            kernel_temp, include_outer_lines=True, arrange_in_grid_config={"col_alignments": "rrr"})
        moving_kernel.set_color(BLUE_D)

        for i, e in enumerate(moving_kernel.get_entries()):
            if i == 6 or i == 8:
                e.set_value(-1)
                e.shift(LEFT*0.4*cell_width)
            e.shift(DOWN*0.5*cell_height + RIGHT*0.7*cell_width)
            e.scale(0.7)

        moving_kernel.scale(SCALE_VALUE)

        kernel_title = Text("Kernel", font_size=15, font="Noto Sans")
        kernel_title.next_to(kernel_table, UP)

        t1 = IntegerTable(np.zeros(image.shape, dtype="uint8"),
                          include_outer_lines=True)
        t1.scale(SCALE_VALUE)
        t1.shift(RIGHT*4)

        t1_title = Text("Hit-Or-Miss Output", font_size=15, font="Noto Sans")
        t1_title.next_to(t1, UP)

        rect_2 = Rectangle(color=BLUE_D, height=cell_height, width=cell_width)

        self.add(scene_title)
        self.play(scene_title.animate.move_to(UP*3))

        self.add(t0)
        self.play(Create(t0))
        self.add(t0_title)

        self.add(t1)
        self.play(Create(t1))
        self.add(t1_title)

        self.add(kernel_table)
        self.play(Create(kernel_table), run_time=RUN_TIME)
        self.add(kernel_title)

        self.add(moving_kernel)
        self.play(moving_kernel.animate.align_to(t0, UL), run_time=RUN_TIME)
        self.add(moving_kernel)
        self.play(moving_kernel.animate.align_to(t0, UL), run_time=RUN_TIME)
        self.add(rect_2)
        self.play(rect_2.animate.align_to(t1, UL).shift(
            RIGHT*cell_width+DOWN*cell_height), run_time=RUN_TIME)

        for i, row in enumerate(image[1:-1], 2):
            for j, pixel in enumerate(row[1:-1], 2):
                sub_image = image[i-2:i+1, j-2:j+1]
                #print((i, j))
                # print(sub_image)
                # print(kernel)

                match = True
                for r in range(0, len(kernel)):
                    for c in range(0, len(kernel[r])):
                        if kernel[r, c] != -1 and sub_image[r, c] != kernel[r, c]:
                            match = False
                            break
                # print(match)
                if match:
                    t1.add_highlighted_cell(
                        (i, j), color=color.WHITE, fill_opacity=1.0)
                    e = t1.get_entries((i, j))
                    e.set_color(color.BLACK)
                    e.set_value(1)
                if j < len(row)-1:
                    self.play(moving_kernel.animate.shift(
                        RIGHT*cell_width), rect_2.animate.shift(
                        RIGHT*cell_width), run_time=RUN_TIME)

            if i < len(image)-1:
                self.play(moving_kernel.animate.shift(
                    DOWN*cell_height).align_to(t0, LEFT), rect_2.animate.align_to(t1, LEFT).shift(
                    DOWN*cell_height+RIGHT*cell_width), run_time=RUN_TIME)
            else:
                self.remove(moving_kernel, rect_2)

        self.wait(1)

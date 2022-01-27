# Skeletonization

This repository cointains all the material (presentation, code, etc...) I developed for the Signal, Image & Video course project.

## Slides
In releases is available the final version of the slides. It is also available in the root as `presentation.pdf`

Please use Adobe Acrobat Reader, Adobe Acrobat DC, PDF-XChange, acroread, or Foxit Reader to open the slides since there are some animations and videos that other pdf readers cannot render. 

Slide 12 "Hit-or-Miss-Trasform" contains a video. To be played, `hit-or-miss.mp4` must be located in the same directory of `presentation.pdf`.

If you want to compile the slides yourself, you need a LaTex distribution with the necessary packages installed.
A simple compilation with pdflatex should be enough to get the pdf.

## Algorithms implementations (Python)

In the root folder of this repo you can find the Jupyter notebook `skeletonizion.ipynb` in which the three algoritms presented in the slides are briefly described, implemented and compared.

In order to run it you need the following steps:
1. Have `python@3.9` installed in your machine.
2. Create a virtual environment `python3 -m venv venv`
3. Activate the virtual environment `source venv/bin/activate`
4. Execute `pip3 install -r requirements.txt` to install all necessary dependencies.
5. Start the jupyter server `jupyter notebook`
6. Enjoy! 

## Parallel Zhang-Suen Implementation (C++)

Since the algorithms presented are parallel algorithms, in order to demonstrate it I implemented a C++ version of Zhang-Suen Algorithm that uses parallel programming techniques.

In the folder `c++` there are three `.cpp` files:
- `zhang.cpp` containing a 1:1 translation from Python to C++ of the algorithm present in the jupyter notebook.
- `zhang_optimized.cpp` containing an optimized version of Zhang-Suen algorithm in C++.
- `zhang_threads.cpp` containing a parallelized version using C++ `std::thread`.

To run it you need to have installed OpenCV library and update `CMakeLists.txt` with your OpenCV installation path on line 12.
To compile and run: 
1. `cd c++/build`
2. `cmake ..` to configure cmake.
3. `make` to compile.
4. `./<executable_name>` to run.

## Manim Hit-or-Miss Transform Animation
I developed an animations to visualize Hit-or-Miss Transform in action using [Manim library](https://www.manim.community/). The relative code is in `hitmiss-animation` folder. 

If you want to re-render it yourserlf you need to install manim and follow the [guide](https://docs.manim.community/en/stable/installation.html#local-installation).

## Step by Step execution images
In the folder `stepbystep_execution` there are images representing the evolution of the skeleton state for each algorithm. There is a folder for each algorithm. 

I used them to make animations available in each folder as `<algorithm_name>.mp4`.

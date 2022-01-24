# Skeletonization

This repository cointains all the material (presentation, code, etc...) I developed for the Signal, Image & Video course project.

## Slides
In releases is available the final version of the slides. It is also available in `presentation/build/presentation.pdf`

If you want to compile it yourself, you need a LaTex distribution with the necessary packages installed.
A simple compilation with pdflatex should be enough to get the pdf.

# Algorithms implementations (Python)

In the root folder of this repo you can find the Jupyter notebook `skeletonizion.ipynb` in which the three algoritms presented in the slides are briefly described, implemented and compared.

In order to run it you need the following steps:
1. Have `python@3.9` installed in your machine.
2. Create a virtual environment `python3 -m venv venv`
3. Activate the virtual environment `source venv/bin/activate`
4. Execute `pip3 install -r requirements.txt` to install all necessary dependencies.
5. Start the jupyter server `jupyter notebook`
6. Enjoy! 

# Parallel Zhang Implementation (C++)

Since the algorithms presented are parallel algorithms, in order to demonstrate it I implemented a C++ version of Zhang-Suen Algorithm that uses parallel programming techniques.

In the folder `c++` there are three `.cpp` files:
- `zhang.cpp` containing a 1:1 translation from python to c++ of the algorithm present in the jupyter notebook.
- `zhang_optimized.cpp` containig an optimized version of Zhang-Suen algorithm in C++.
- `zhang_threads.cpp` containing a parallelized version using C++ `std::thread`.

To run it you need to have installed OpenCV library and update `CMakeLists.txt` with your OpenCV installation path on line 12.
To compile and run: 
1. `cd c++/build`
2. `cmake ..` to configure cmake.
3. `make` to compile.
4. `./<executable_name>` to run.

## Step by Step execution images
In the folder `stepbystep_execution` there are images representing the evolution of the skeleton state for each algorithm. There is a folder for each algorithm. 

I used them to make animations available in each folder as `<algorithm_name>.mp4`.

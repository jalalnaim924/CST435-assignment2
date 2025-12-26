CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall
OPENCV_FLAGS = $(shell pkg-config --cflags --libs opencv4)
OPENMP_FLAGS = -fopenmp

SRCDIR = src
BINDIR = bin
OBJDIR = obj

# Common object files
COMMON_OBJS = $(OBJDIR)/filters.o $(OBJDIR)/image_io.o

# Create directories
$(shell mkdir -p $(BINDIR) $(OBJDIR))

.PHONY: all clean threads openmp

all: threads openmp

# std::thread version
threads: $(BINDIR)/image_processor_threads

$(BINDIR)/image_processor_threads: $(OBJDIR)/threads_version.o $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ $(OPENCV_FLAGS) -pthread

$(OBJDIR)/threads_version.o: $(SRCDIR)/threads_version.cpp
	$(CXX) $(CXXFLAGS) $(OPENCV_FLAGS) -c $< -o $@

# OpenMP version
openmp: $(BINDIR)/image_processor_openmp

$(BINDIR)/image_processor_openmp: $(OBJDIR)/openmp_version.o $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) $(OPENMP_FLAGS) -o $@ $^ $(OPENCV_FLAGS)

$(OBJDIR)/openmp_version.o: $(SRCDIR)/openmp_version.cpp
	$(CXX) $(CXXFLAGS) $(OPENMP_FLAGS) $(OPENCV_FLAGS) -c $< -o $@

# Common object files
$(OBJDIR)/filters.o: $(SRCDIR)/filters.cpp $(SRCDIR)/filters.h
	$(CXX) $(CXXFLAGS) $(OPENCV_FLAGS) -c $< -o $@

$(OBJDIR)/image_io.o: $(SRCDIR)/image_io.cpp $(SRCDIR)/image_io.h
	$(CXX) $(CXXFLAGS) $(OPENCV_FLAGS) -c $< -o $@

clean:
	rm -rf $(BINDIR)/* $(OBJDIR)/*
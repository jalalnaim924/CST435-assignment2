CXX = g++
CXXFLAGS = -std=c++17 -O3 -Wall -Iinclude
OPENMP_FLAGS = -fopenmp

SRCDIR = src
BINDIR = bin
OBJDIR = obj

# Common object files
COMMON_OBJS = $(OBJDIR)/filters.o $(OBJDIR)/image_io.o

# Create directories
$(shell mkdir -p $(BINDIR) $(OBJDIR))

.PHONY: all clean threads openmp sequential

all: sequential threads openmp

# Sequential version
sequential: $(BINDIR)/image_processor_sequential

$(BINDIR)/image_processor_sequential: $(OBJDIR)/main.o $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -pthread

$(OBJDIR)/main.o: $(SRCDIR)/main.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# std::thread version
threads: $(BINDIR)/image_processor_threads

$(BINDIR)/image_processor_threads: $(OBJDIR)/threads_version.o $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) -o $@ $^ -pthread

$(OBJDIR)/threads_version.o: $(SRCDIR)/threads_version.cpp
	$(CXX) $(CXXFLAGS) -c $< -o $@

# OpenMP version
openmp: $(BINDIR)/image_processor_openmp

$(BINDIR)/image_processor_openmp: $(OBJDIR)/openmp_version.o $(COMMON_OBJS)
	$(CXX) $(CXXFLAGS) $(OPENMP_FLAGS) -o $@ $^ -pthread

$(OBJDIR)/openmp_version.o: $(SRCDIR)/openmp_version.cpp
	$(CXX) $(CXXFLAGS) $(OPENMP_FLAGS) -c $< -o $@

# Common object files
$(OBJDIR)/filters.o: $(SRCDIR)/filters.cpp $(SRCDIR)/filters.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

$(OBJDIR)/image_io.o: $(SRCDIR)/image_io.cpp $(SRCDIR)/image_io.h
	$(CXX) $(CXXFLAGS) -c $< -o $@

clean:
	rm -rf $(BINDIR)/* $(OBJDIR)/*

run-sequential: sequential
	./$(BINDIR)/image_processor_sequential

run-threads: threads
	./$(BINDIR)/image_processor_threads

run-openmp: openmp
	./$(BINDIR)/image_processor_openmp

run-all: all
	@echo "Running sequential version..."
	./$(BINDIR)/image_processor_sequential
	@echo "\nRunning threads version..."
	./$(BINDIR)/image_processor_threads
	@echo "\nRunning OpenMP version..."
	./$(BINDIR)/image_processor_openmp
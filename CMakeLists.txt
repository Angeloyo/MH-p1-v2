cmake_minimum_required(VERSION 3.31)

PROJECT(mh_p1 CXX)

file(GLOB C_SOURCES
  "src/*.cpp"
)
INCLUDE_DIRECTORIES("common" "inc")

ADD_EXECUTABLE(main "main.cpp" ${C_SOURCES})
ADD_EXECUTABLE(test_random "test_random.cpp" ${C_SOURCES})
ADD_EXECUTABLE(test_greedy "test_greedy.cpp" ${C_SOURCES})
ADD_EXECUTABLE(test_localsearch "test_localsearch.cpp" ${C_SOURCES})
ADD_EXECUTABLE(test_all "test_all.cpp" ${C_SOURCES})
ADD_EXECUTABLE(run_experiments "run_experiments.cpp" ${C_SOURCES})
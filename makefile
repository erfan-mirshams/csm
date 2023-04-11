##
# CSM
#
# @file
# @version 0.1

output: csm.cpp
g++ -std=c++11 -Wall -Wextra -fsanitize=address -fsanitize=leak csm.cpp

# end

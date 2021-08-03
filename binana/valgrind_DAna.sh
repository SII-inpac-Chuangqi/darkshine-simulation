#!/bin/bash
source /home/zhuyifan/DarkShine/dp.env

cd /home/zhuyifan/DarkShine/Simulation/darkshine-simulation/source/binana/
#valgrind --leak-check=yes --track-origins=yes --xml=yes --xml-file=memory.xml DAna -c config.txt
valgrind --tool=memcheck --leak-check=full DAna -c config.txt

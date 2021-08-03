#!/bin/bash
source /home/zhuyifan/DarkShine/dp.env

cd /home/zhuyifan/DarkShine/Simulation/darkshine-simulation/source/binana/

DAna -c config.txt > output.txt 2>&1

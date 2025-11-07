build:
	rm -f cs
	mpic++ -O2 -Wall main.cpp bitmap.cpp cs-util.cpp mainp.cpp workerp_mid.cpp workerp_end.cpp -o cs -Wno-unused-but-set-variable -Wno-unused-function -Wno-write-strings -Wno-unused-result

test-mid:
	rm -f cs
	mpic++ -O2 -Wall main.cpp bitmap.cpp cs-util.cpp mainp.o workerp_mid.cpp workerp_end.o -o cs -Wno-unused-but-set-variable -Wno-unused-function -Wno-write-strings -Wno-unused-result

test-ends:
	rm -f cs
	mpic++ -O2 -Wall main.cpp bitmap.cpp cs-util.cpp mainp.cpp workerp_mid.o workerp_end.cpp -o cs -Wno-unused-but-set-variable -Wno-unused-function -Wno-write-strings -Wno-unused-result

run:
	rm -f temp.bmp
	mpiexec --oversubscribe -n 4 ./cs sunset.bmp temp.bmp 10
	diff temp.bmp sunset-10.bmp

run-5:
	rm -f temp.bmp
	mpiexec --oversubscribe -n 5 ./cs sunset.bmp temp.bmp 10
	diff temp.bmp sunset-10.bmp

run-75:
	rm -f temp.bmp
	mpiexec --oversubscribe -n 4 ./cs sunset.bmp temp.bmp 75
	diff temp.bmp sunset-75.bmp
  
run-75-11:
	rm -f temp.bmp
	mpiexec --oversubscribe -n 11 ./cs sunset.bmp temp.bmp 75
	diff temp.bmp sunset-75.bmp
  
submit:
	/gradescope/gs submit 1130318 7114078 *.cpp

default:
	g++ main.cpp alg_one.cpp alg_two.cpp alg_three.cpp face_detect.cpp -o secure_ur_face `pkg-config --cflags --libs opencv` -O3 -std=c++11

clean:
	rm secure_ur_face

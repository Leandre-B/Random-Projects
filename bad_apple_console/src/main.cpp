#include <codecvt>
#include <opencv2/opencv.hpp>
#include <ratio>
#include <stdio.h>

#include <chrono>
#include <thread>

#include <unistd.h> //sleep linux

int main(int argc, char *argv[]) {
  // open the video file for reading
  std::string file_name=argv[1];
  file_name = argv[1];
  cv::VideoCapture cap("../src/"+file_name);

  // if not success, exit program
  if (cap.isOpened() == false) {
    std::cout << "Cannot open the video file" << std::endl;
    std::cin.get(); // wait for any key press
    return -1;
  }

  // get the frames rate of the video
  double fps = cap.get(cv::CAP_PROP_FPS);
  int time_btw_frame = (1000.0 / fps); // en ms

  cv::Mat frame, resized, gray, binary;

  int totalFrames = static_cast<int>(cap.get(cv::CAP_PROP_FRAME_COUNT));

  while (true) {
    auto start=std::chrono::high_resolution_clock::now();

    cap >> frame;
    float new_size = 70.0f/std::max(frame.rows, frame.cols); 
    // resize
    cv::resize(frame, frame, cv::Size(), new_size, new_size, cv::INTER_AREA);

    // on parcours tout les pixel de la frame
    for (int y = 0; y < frame.rows; y++) {
      for (int x = 0; x < frame.cols; x++) {

        //couleur du pixel (BGR)
        cv::Vec3b color = frame.at<cv::Vec3b>(y, x);

        int B = color[0];
        int G = color[1];
        int R = color[2];

        //calcul le niveau de luminosité
        int luminosity = 0.299 * R + 0.587 * G + 0.114 * B;

        //symbole en fonction de la luminosité
        if(luminosity>220)
            std::cout<<'@';
        else if(luminosity>200)
            std::cout<<'0';
        else if(luminosity>150)
            std::cout<<'O';
        else if(luminosity>100)
            std::cout<<'.';
        else
          std::cout<<' ';
      }
      std::cout<<std::endl;
    }
    std::cout<<"\x1b[2J";
    auto stop = std::chrono::high_resolution_clock::now();

    std::chrono::duration<double, std::milli> duree = stop - start;
    int duree_ms = duree.count();
    std::this_thread::sleep_for(std::chrono::milliseconds(time_btw_frame-duree_ms));
  }
  return 0;
}
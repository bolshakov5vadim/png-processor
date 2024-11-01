#define _CRT_SECURE_NO_WARNINGS
#include <iostream>
#include <string>
#include <fstream>
#include <filesystem>
#include <vector>
#include <opencv2/opencv.hpp>

namespace fs = std::filesystem;

int main()
{
    setlocale(LC_ALL, "Russian");
    std::cout << "Будет обработана папка folder рядом с EXE" << std::endl;
    std::cout << "1-инвертирование 2-обесцвечивание 3-контуры" << std::endl;
    std::cout << "4-квадратура обрезкой 5-квадратура достройкой" << std::endl;
    int flag;
    std::cin >> flag;

    std::string pathh = "folder";
    for (const auto& path : fs::directory_iterator(pathh))
    {
    cv::Mat image = cv::imread(path, cv::IMREAD_COLOR);
    if (flag == 1) { cv::bitwise_not(image, image); }
    if (flag == 2) { image = cv::imread(path, cv::IMREAD_GRAYSCALE); }
    //
    if (flag == 3) 
    {
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
        cv::threshold(image, image, 128, 255, cv::THRESH_BINARY);

        //cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, { 5,5 });
        // erode/rect-
        //cv::morphologyEx(image, image, cv::MORPH_GRADIENT, kernel);
        // findContours
        // findContours

        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(image, contours, hierarchy, cv::RETR_TREE, cv::CHAIN_APPROX_NONE);
        //

        cv::Mat newimage(image.cols, image.rows, CV_8UC1, (255, 255, 255));
        cv::drawContours(newimage, contours, -1, cv::Scalar(0, 0, 0), 1);
        //
        image = newimage;
        cv::cvtColor(image, image, cv::COLOR_BGR2GRAY);
        //cv::imshow("проверка конт", image);
        //cv::waitKey(0);
        //cv::destroyAllWindows();
    }

    if (flag == 4) 
    {
        cv::Mat gray;  cv::cvtColor(image, gray, cv::COLOR_BGR2GRAY);
        cv::Mat threshold;
        cv::adaptiveThreshold(gray, threshold, 255, cv::ADAPTIVE_THRESH_MEAN_C, cv::THRESH_BINARY, 45, 15);
        cv::Mat kernel = cv::getStructuringElement(cv::MORPH_ELLIPSE, { 5,5 });
        //
        cv::morphologyEx(threshold, threshold, cv::MORPH_GRADIENT, kernel);
        //


        std::vector<std::vector<cv::Point> > contours;
        std::vector<cv::Vec4i> hierarchy;
        cv::findContours(threshold, contours, hierarchy, cv::RETR_EXTERNAL, cv::CHAIN_APPROX_SIMPLE);
        //


        int dioptria= image.rows * image.rows / 15;//
        cv::Rect rect;
        cv::Mat mask(image.rows, image.cols, CV_8UC1, (0, 0, 0));

        //
        for (int i = 0; i < contours.size(); i++)
        {
            double area = cv::contourArea(contours[i]);
            if (area > dioptria)
            {rect = cv::boundingRect(contours[i]);}
        }
        image = image(rect);

    }
    if (flag == 5) 
    {                                                                                                                if(image.rows < image.cols)
        {
            cv::Mat row = cv::Mat::ones((image.cols-image.rows)/2, image.cols, CV_8UC1);
            cv::cvtColor(row, row, cv::COLOR_GRAY2BGR);
            cv::Mat result;
            result.push_back(row);
            result.push_back(image);
            result.push_back(row);
            image = result;
        }

        else//
        {
            cv::Mat col = cv::Mat::ones(image.rows, (image.rows - image.cols) / 2, CV_8UC1);
            if (col.empty() != 1)
            {
            cv::cvtColor(col, col, cv::COLOR_GRAY2BGR);

            cv::Mat result(image.rows, image.cols + col.cols + col.cols, image.type());

            col.copyTo(result(cv::Rect(0, 0, col.cols, col.rows)));
            image.copyTo(result(cv::Rect(col.cols, 0, image.cols, image.rows)));
            col.copyTo(result(cv::Rect(col.cols + image.cols, 0, col.cols, col.rows)));
            image = result;
            }
        }
    }

        cv::Mat threshold;  cv::threshold(image, threshold, 1, 255, cv::THRESH_BINARY_INV);

        cv::inpaint(image, threshold, image, 3, cv::INPAINT_NS); //

        cv::resize(image, image, { side, side }, 0, 0, cv::INTER_LINEAR);

        cv::imshow("experiment", image);
        cv::waitKey(0);
        cv::destroyAllWindows();

    }
    if (image.empty() != 1) { cv::imwrite(path, image); }
    //
}
std::cout << "Обработка завершена";
system("pause");
return 0;
}



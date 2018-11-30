#pragma once

#include <cassert>
#include <chrono>
#include <iostream>

using namespace std::chrono;

class SvgFileGenerator {

public:
    inline explicit SvgFileGenerator( const char* fileName = "default.svg" ) {
        svgFile = fopen( fileName, "w" );
        assert( svgFile != nullptr );
        fprintf( svgFile, "<\?xml version=\"1.0\" encoding=\"UTF-8\" standalone=\"no\"\?>\n"
                        "<svg  version=\"1.1\" width=\"1280\" height=\"1024\"\n"
                        "\tviewBox=\"0 0 1280 1024\"\n"
                        "\tbaseProfile=\"full\"\n"
                        "\txmlns=\"http://www.w3.org/2000/svg\"\n"
                        "\txmlns:xlink=\"http://www.w3.org/1999/xlink\"\n"
                        "\txmlns:ev=\"http://www.w3.org/2001/xml-events\">\n\n"
                        "\t<title>My first SVG file</title>\n"
                        "\t\t<g>\n"
                        "\t\t\t<rect x=\"0\" y=\"0\" width=\"1280\" height=\"1024\" fill=\"white\"/>");
    }
    inline void WriteLine( double x1, double y1, double x2, double y2, const char* color = "black" ) {
        fprintf( svgFile, "\t\t\t<line x1=\"%lf\" y1=\"%lf\" x2=\"%lf\" y2=\"%lf\" stroke=\"%s\"/>\n",
                 x1, y1, x2, y2, color );
    }
    ~SvgFileGenerator() {
        fprintf( svgFile, "\t</g>\n</svg>\n" );
        fclose( svgFile );
    }
private:
    FILE* svgFile;
};

class MyTimer {

public:
    typedef high_resolution_clock hr_clock;
    inline void InitTime() { begin = hr_clock::now().time_since_epoch(); }
    inline void GetTime() const { std::cout << duration_cast<nanoseconds>( hr_clock::now().time_since_epoch() - begin ).count() / 1e9 << std::endl; }
private:
    duration<int64_t, std::nano> begin;
};

class Auxilliary : public SvgFileGenerator, public MyTimer {};


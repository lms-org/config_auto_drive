#include "sobel_threshold_generator.h"

bool SobelThresholdGenerator::initialize() {

    target = readChannel<lms::imaging::Image>("IMAGE");

    return true;
}

bool SobelThresholdGenerator::deinitialize() {
    return true;
}

bool SobelThresholdGenerator::cycle() {

    logger.time("sobel");

    sobelBuffer.resize(target->width(), target->height(), target->format());


    if (target->width() > 0) {
        //logger.debug("Sobel") << "image 0,0: " << I[0];

        const uint8_t *I = target->data(); //uint8
        uint8_t *buffer = sobelBuffer.data();

        int numel = target->width() * target->height();
        //int numBorder = 2*target->height() + 2*target->width() - 4;
        int numel_red = (target->width()-2) * (target->height()-2);

        for (int i=0; i < numel; ++i) {
            buffer[i] = 0;
        }

        int startRow = 120;
        int endRow = 250;
        int startCol = 100;
        int endCol = 600;

        //wieviele 0er exisitieren als "rand" im sobelbild
        //int numBorder = 2*target->height() + 2*target->width() - 4; //aeusserer Rand
        int numBorder = startRow*target->width() + (target->height() - endRow)*target->width();
        int restHeight = endRow - startRow;
        //numBorder += startCol*restHeight + (target->width() - endCol)*restHeight;

        sobelWithoutSqrt( I, buffer,  target->width(), target->height(), startRow, endRow);



        //double percBorder = (double(numel-numel_red))/double(numel);

        logger.debug("sobel n_th") << (int)(((int)(0.95*numel_red + numBorder)));

        std::nth_element (buffer, buffer+((int)(0.95*numel_red + numBorder)), buffer + numel);
        //std::sort(buffer, buffer + numel);

        int zeroCountSobel = 0, zeroCountImage = 0;
        for (int i =0; i< numel; ++i) {
            //std::cout << (int)(buffer[i])<< " ; ";
            if ((int)(buffer[i]) == 0) zeroCountSobel++;
            if (!i%target->width()) std::cout << "\n";
        }
        for (int i =0; i< numel; ++i) {
            //std::cout << (int)(buffer[i])<< " ; ";
            if ((int)(I[i]) == 0) zeroCountImage++;
            if (!i%target->width()) std::cout << "\n";
        }

        double threshold = (6.0 * ((double)(buffer[(int)(0.95*numel_red + numBorder)])));

        //logger.debug("Sobel") << "width: " << target->width() << "  height: " << target->height();
        logger.debug("Sobel") << "threshold: " << threshold;
        logger.debug("Sobel") << "zeroCountImage: " << zeroCountImage   << "  zeroCountSobel: " << zeroCountSobel;
    }

    //logger.debug("Sobel") << "time: " ;
    logger.timeEnd("sobel");



    return true;
}

void SobelThresholdGenerator::sobelWithoutSqrt(const uint8_t *imageIn, uint8_t *imageOut,  int nc, int nr, int startRow, int endRow) {
    int ncm1 = nc - 1, nrm1 = nr - 1;
    //int8_t k00 = k[ 0 ], k01 = k[ 1 ], k02 = k[ 2 ], k10 = k[ 3 ], k11 = k[ 4 ], k12 = k[ 5 ], k20 = k[ 6 ], k21 = k[ 7 ], k22 = k[ 8 ];

    for ( int ii = startRow; ii < endRow-1; ii++ ) {
        const uint8_t *a = imageIn + ( ii - 1 ) * nc;
        const uint8_t *b = a + 1;
        const uint8_t *c = b + 1;
        const uint8_t *d = a + nc;
        const uint8_t *e = d + 1;
        const uint8_t *f = e + 1;
        const uint8_t *g = d + nc;
        const uint8_t *h = g + 1;
        const uint8_t *i = h + 1;
        uint8_t *rOut = imageOut + ii * nc + 1;
        for ( int j = 1; j < ncm1; j++ ) {
            *rOut++ = (uint8_t)(sqrt((pow((int)(*a) -    (int)(*c) + 2* (int)(*d) - 2* (int)(*f) +    (int)(*g) - (int)(*i), 2)
                                   + pow((int)(*a) + 2* (int)(*b) +    (int)(*c) -    (int)(*g) - 2* (int)(*h) - (int)(*i), 2)))/6.0);

            ++a; ++b; ++c; ++d; ++e; ++f; ++g; ++h; ++i;
            /* *rOut++ = ( k00 * *a++ ) + ( k01 * *b++ ) + ( k02 * *c++ ) +
                      ( k10 * *d++ ) + ( k11 * *e++ ) + ( k12 * *f++ ) +
                      ( k20 * *g++ ) + ( k21 * *h++ ) + ( k22 * *i++ );*/
        }
    }

    /*for ( int ii = 1; ii < nrm1; ii++ ) {
        const uint8_t *a = imageIn + ( ii - 1 ) * nc;
        const uint8_t *b = a + 1;
        const uint8_t *c = b + 1;
        const uint8_t *d = a + nc;
        const uint8_t *e = d + 1;
        const uint8_t *f = e + 1;
        const uint8_t *g = d + nc;
        const uint8_t *h = g + 1;
        const uint8_t *i = h + 1;
        uint8_t *rOut = imageOut + ii * nc + 1;
        for ( int j = 1; j < ncm1; j++ ) {
            *rOut++ = (uint8_t)(sqrt((pow((int)(*a) -    (int)(*c) + 2* (int)(*d) - 2* (int)(*f) +    (int)(*g) - (int)(*i), 2)
                                   + pow((int)(*a) + 2* (int)(*b) +    (int)(*c) -    (int)(*g) - 2* (int)(*h) - (int)(*i), 2)))/6.0);

            ++a; ++b; ++c; ++d; ++e; ++f; ++g; ++h; ++i;

        }
    }*/
}

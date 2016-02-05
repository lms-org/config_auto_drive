#include "sobel_threshold_generator.h"


bool SobelThresholdGenerator::initialize() {

    target = readChannel<lms::imaging::Image>("IMAGE");

    std::pair<double, CountType> p;
    p.first = 0.0;
    p.second = 0;
    fisherData.assign(100, p);
    calculateThreshold = true;

    cycleCount = 0;

    resetFisherData();

    return true;
}

bool SobelThresholdGenerator::deinitialize() {
    return true;
}

bool SobelThresholdGenerator::cycle() {



    /*
     * Grundidee:
     * 1. gewünschten Bildausschnitt sobeln und ein "vereinfachtes Histogramm" dieser Werte bilden (100 Balken in 'fisherData');
     * 2. auf diesem Histogramm den Fisher Breaks Algorithmus anwenden (der clustert (analog zu K-Means für 1D-Daten) die Werte in 2 Kategorien und
     * liefert den Punkt, von dem die beiden cluster-Zentren maximal weit entfernt sind => Threshold für Sobel)
     *
     */   

    if (calculateThreshold)
    {

        sobelBuffer.resize(target->width(), target->height(), target->format());

        if (target->width() > 0) {

            calculateThreshold = false;

            resetFisherData();

            const uint8_t *I = target->data(); //uint8

            //Bildausschnitt festlegen
            //int startRow = 120;
            //int endRow = 250;
            int startRow = target->height()/4;
            int endRow = target->height()/2;

            fisherHistogram(I, target->width(), target->height(), startRow, endRow);

            LimitsContainer resultingbreaksArray;
            FisherBreaks::ClassifyJenksFisherFromValueCountPairs(resultingbreaksArray, 2, fisherData);

            int sobelThreshold = 10*resultingbreaksArray.at(1);
            logger.info("sobel") << "sobelThreshold = " << sobelThreshold;

        }


    }

    return true;
}

void SobelThresholdGenerator::resetFisherData() {

    std::fill(fisherHistData.begin(), fisherHistData.end(), 1);
    //------------------------------------------------------------
    for (int i=0; i < 100; ++i)
    {
        fisherData.at(i).first = i;
        fisherData.at(i).second = 0;
    }
}


void SobelThresholdGenerator::fisherHistogram(const uint8_t *imageIn, int nc, int nr, int startRow, int endRow)
{
    int ncm1 = nc - 1, nrm1 = nr - 1, histBin;
    double sob;
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

        for ( int j = 1; j < ncm1; j++ ) {
            sob = sqrt((pow((int)(*a) -    (int)(*c) + 2* (int)(*d) - 2* (int)(*f) +    (int)(*g) - (int)(*i), 2)
                                   + pow((int)(*a) + 2* (int)(*b) +    (int)(*c) -    (int)(*g) - 2* (int)(*h) - (int)(*i), 2)));
            ++a; ++b; ++c; ++d; ++e; ++f; ++g; ++h; ++i;

            histBin = (int)std::max(0.0, std::min(99.0, std::round(sob/10)));
            fisherData.at(histBin).second += 1;
        }
    }
}


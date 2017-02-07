#ifndef DETECTION_UTILS_H
#define DETECTION_UTILS_H
#include <lms/math/vertex.h>
#include <lms/imaging/image.h>
#include <lms/imaging/graphics.h>
#include <lms/imaging/transform_image.h>
#include <lms/logger.h>

inline std::vector<lms::math::vertex2f> findBySobel(
        const lms::imaging::Image *image,
        lms::imaging::Image *debugImage,
        const bool renderDebugImage,
        const std::vector<int> &xv,
        const std::vector<int> &yv,
        const float minLineWidthMul,
        const float maxLineWidthMul,
        const float lineWidth,
        const float iDist,
        const float wDist,
        const int threshold,
        lms::imaging::Transformation &trans) {
    //lms::logging::Logger logger("findBySobel");

    lms::imaging::BGRAImageGraphics graphics(*debugImage);
    std::vector<lms::math::vertex2f> foundPoints;
    bool foundLowHigh = false;
    int pxlCounter = 0;
    for(int k = 1; k < (int)xv.size()-1; k++){
        const int xs = xv[k-1];
        const int ys = yv[k-1];
        const int x = xv[k];
        const int y = yv[k];
        const int xS = xv[k+1];
        const int yS = yv[k+1];
        //check if point is inside the image
        if(!image->inside(xs,ys) || !image->inside(x,y) || !image->inside(xS,yS)){
            continue;
        }
        if(renderDebugImage){
            graphics.setColor(lms::imaging::blue);
            graphics.drawCross(x,y);
        }

        int colors = *(image->data()+xs+ys*image->width());
        int colorS = *(image->data()+xS+yS*image->width());
        int sobel = colorS-colors;
        //da wir von links nach rechts suchen ist positiver sobel ein dunkel-hell übergang
        if(sobel > threshold){
            if(!foundLowHigh){
                foundLowHigh = true;
                //logger.debug("")<<"crossing found lowHigh"<<std::endl;
                pxlCounter = 0;
            }
            if(renderDebugImage){
                graphics.setColor(lms::imaging::green);
                graphics.drawCross(x,y,3);
            }
        }else if(sobel < -threshold){ //hell-dunkel übergang
            if(renderDebugImage){
                graphics.setColor(lms::imaging::yellow);
                graphics.drawCross(x,y,3);
            }
            //check if we found a lowHigh + highLow border
            if(foundLowHigh){
                //check if the points have the right distance
                float pxlPeakWidth = iDist/wDist*lineWidth; //TODO to bad, calculate for each road line (how should we use them for searching?

                //logger.debug("")<<"crossing found highLow: "<<pxlCounter<<" "<<pxlPeakWidth;
                //logger.debug("")<<"crossing found max: "<<pxlPeakWidth*maxLineWidthMul;
                //logger.debug("")<<"crossing found min: "<<pxlPeakWidth*minLineWidthMul;

                if(pxlCounter > pxlPeakWidth*minLineWidthMul && pxlCounter < pxlPeakWidth*maxLineWidthMul){
                    //we found a valid poit, mark it
                    if(renderDebugImage){
                        graphics.setColor(lms::imaging::red);
                        for(int j = 0; j<pxlCounter;j++){
                            graphics.drawCross(xv[k-j],yv[k-j]);
                        }
                    }
                    //we found a valid point
                    //get the middle
                    lms::math::vertex2f wMid;
                    trans.t(xv[k-pxlCounter/2],yv[k-pxlCounter/2],wMid.x,wMid.y);
                    foundPoints.push_back(wMid);
                    //logger.debug("")<<"crossing FOUND VALID CROSSING";
                }
            }
            if(renderDebugImage && pxlCounter > 0){
                graphics.setColor(lms::imaging::green);
                graphics.drawCross(x,y);
            }
            pxlCounter = 0;
            foundLowHigh = false;
            //if not, we dont have to do anything
        }
        if(foundLowHigh){
            pxlCounter++;
        }
    }
    return foundPoints;
}

inline void getXYfromPoint(const lms::math::vertex2f &bot,const lms::math::vertex2f &top,std::vector<int> &xv,std::vector<int> &yv,const lms::imaging::Homography &homo){
    xv.clear();
    yv.clear();
    lms::math::vertex2i iBot,iTop;
    homo.vti(iBot,bot);
    homo.vti(iTop,top);
    lms::math::bresenhamLine(iBot.x,iBot.y,iTop.x,iTop.y,xv,yv);
}

#endif //DETECTION_UTILS_H

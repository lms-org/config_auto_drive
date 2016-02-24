# image_object_renderer
Draws objects on a rgb-image

## Formats

## DataChannels
- IMAGE draws on it
- envObjects (list in config)

## Config
```
    <module>
        <name>image_object_renderer</name>
        <channelMapping from="IMAGE" to="IMAGE_OBJECT_IMAGE"/>
        <config src="street_utils/obstacle.lconf"/>
        <config>
            <envObjects>MIDDLE_LANE,TRAJECTORY_LINE,ENVIRONMENT_OBSTACLE,TRAJECTORY_POINT,VISIBLE_AREAS,DEBUG_POINTS_RAW,DEBUG_POINTS</envObjects><!--DEBUG_TRAJECTORY, DEBUG_TRAJECTORY_2, -->
            <!--The default Color, TODO add colors for points, lines etc -->
            <colorR>255</colorR>
            <colorG>255</colorG>
            <colorB>0</colorB>
        </config>
        <config name="DEBUG_POINTS">
            <attr>point</attr>
            <colorR>255</colorR>
            <colorG>0</colorG>
            <colorB>125</colorB>
        </config>
        <config name="DEBUG_POINTS_RAW">
            <attr>point</attr>
            <colorR>0</colorR>
            <colorG>0</colorG>
            <colorB>255</colorB>
        </config>
        <config name="START_LINE">
            <colorR>0</colorR>
            <colorG>255</colorG>
            <colorB>0</colorB>
        </config>
        <config name="CROSSING">
            <colorR>255</colorR>
            <colorG>0</colorG>
            <colorB>0</colorB>
        </config>
        <config name="CROSSING_NOT_TRUSTED">
            <colorR>0</colorR>
            <colorG>0</colorG>
            <colorB>0</colorB>
        </config>
        <config name="MIDDLE_LANE">
            <colorR>255</colorR>
            <colorG>123</colorG>
            <colorB>120</colorB>
        </config>
        <config name="DEFAULT_OBSTACLE">
            <colorR>0</colorR>
            <colorG>0</colorG>
            <colorB>255</colorB>
        </config>
        <config name="OBSTACLE_DETECTED">
            <colorR>255</colorR>
            <colorG>255</colorG>
            <colorB>0</colorB>
        </config>
        <config name="DEBUG_TRAJECTORY">
            <colorR>255</colorR>
            <colorG>255</colorG>
            <colorB>0</colorB>
        </config>
        <config name="TRAJECTORY_POINT">
            <colorR>255</colorR>
            <colorG>0</colorG>
            <colorB>0</colorB>
        </config>
        <config name="SENSOR_LINE">
            <colorR>255</colorR>
            <colorG>0</colorG>
            <colorB>123</colorB>
        </config>
    </module>
```
## Dependencies
- [imaging](https://github.com/syxolk/imaging)

## Recommended modules
- [image_converter](https://github.com/syxolk/image_converter) - to convert the read images to other formats
- [camera_renderer](https://github.com/Phibedy/camera_renderer) - to visualize the loaded images

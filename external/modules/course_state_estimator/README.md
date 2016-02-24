# course_state_estimator
Schätzt den Straßenverlauf Mithilfe eines Hidden Markov Models ab.
Es gibt die Zustände StraightToStraightCurve,StraightCurveToCurve,CurveToStraight
## Data channels
- ROAD street_environment::RoadLane
- ROAD_STATES street_environment::RoadState
- ENVIRONMENT street_environment::EnvironmentObjects (Rohdaten mithilfe welcher die ROAD erstellt wurde

## Config
```
    <module>
        <name>course_state_estimator</name>
        <channelMapping from="ROAD" to="MIDDLE_LANE"/>
        <channelMapping from="ENVIRONMENT" to="ENVIRONMENT_ROAD_RAW"/>
        <config>
        <observationProbability1>0.6</observationProbability1>
            <observationProbability2>0.7</observationProbability2>
            <distance1>0.4</distance1>
            <distance2>0.8</distance2>
            <distance3>0.8</distance3>
            <distance4>1.4</distance4>
            <curvatureThreshold>0.2</curvatureThreshold>
            <transitionProbabilityStraightToStraightCurve>0.002</transitionProbabilityStraightToStraightCurve>
            <transitionProbabilityStraightCurveToCurve>0.002</transitionProbabilityStraightCurveToCurve>
            <transitionProbabilityCurveToStraight>0.001</transitionProbabilityCurveToStraight>
            <useKalmanCurvature>true</useKalmanCurvature>
            <useProbabilitiesStraight>true</useProbabilitiesStraight>
         </config>
    </module>
```


## Dependencies
 * EIGEN3
 * street_utils

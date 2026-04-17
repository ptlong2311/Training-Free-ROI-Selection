# Autonomous Mobile Robot 300kg

An industrial Autonomous Mobile Robot (AMR) designed for transporting KIA Cerato exhaust pipes inside the THACO Mechanical Complex. This project focuses on replacing labor-intensive and inflexible towing-based transport with a compact lifting AMR that can autonomously navigate, avoid obstacles, and handle payload transfer more efficiently.

## Overview

This thesis project addresses a real manufacturing problem: transporting hollow exhaust pipes from the storage area to the welding section over a distance of about 60 meters in a factory environment. The previous towing-style transport workflow depended on carts and fixed transport assumptions, which reduced flexibility and increased operational complexity.

To solve this, the project proposes a lifting-type AMR with natural navigation, onboard sensing, autonomous motion control, and a scissor-lift mechanism for direct payload handling. The robot was designed to improve productivity, safety, adaptability, and long-term scalability in dynamic factory environments.

## Objectives

- Design a compact AMR capable of carrying and lifting industrial payloads up to 300 kg.
- Replace towing-based internal logistics with a more flexible carrying-style robot.
- Develop a mechanical platform with differential drive and a scissor-lift system.
- Implement autonomous navigation using ROS, SLAM, localization, and obstacle avoidance.
- Build a reliable motor control pipeline using encoder feedback and PID control.
- Integrate PLC-based lift control and safety mechanisms for industrial operation.

## Key Features

- Payload capacity: 300 kg.
- Battery system: 24VDC – 70Ah.
- Speed target: 0.2–1 m/s.
- Natural navigation with mapping flexibility.
- LiDAR-based safety and obstacle detection.
- Computer-based monitoring and control.
- Automatic battery and charging alerts.
- Speaker and tower-light safety alerts.

## System Design

### Mechanical Design

The robot uses a differential-drive configuration with two rear driving wheels and one front castor wheel, selected for simplicity, maneuverability, and suitability for flat indoor factory floors. The drive system uses a gearbox-based transmission, and the frame was validated through structural stress simulation.

A scissor-lift mechanism was designed to support vertical handling of loads, powered by a 24V electric hydraulic cylinder. The lifting platform supports a 300 kg load with a safety factor of 1.25, and the minimum actuator pushing force was calculated to be 5202 N.

### Control and Electronics

The controller architecture includes an ATmega2560-based board for motion-related functions and a PLC for lift actuation. The electrical system also includes relays, emergency stop logic, DC-DC converters, and optocoupler-based isolation to improve reliability and reduce EMI.

The lift system uses limit switches and PLC logic to guarantee safe upper and lower stopping conditions during extension and retraction. This improves both automation reliability and operator safety in industrial use.

### Navigation and Robotics Software

The software stack is based on ROS, with SLAM for mapping unknown environments and ROS Navigation Stack components for localization, planning, and execution. The system uses packages and concepts such as ROS Master, nodes, topics, AMCL, gmapping/cartographer, move_base, global planners, local planners, and costmaps.

Robot simulation was conducted in Ubuntu ROS Noetic with Gazebo, where LiDAR-based map generation and navigation workflows were tested before practical deployment.

### Motor Control

The project uses Hall-effect encoder feedback for motor position and direction tracking. Encoder values were read using inline assembly to reduce latency and improve responsiveness.

For motor modeling, the transfer function was obtained through system identification in MATLAB, and PID control was selected as the most effective controller configuration for stable and accurate motor performance. Autotuning further improved control quality without heavy manual retuning.

## Results

The lifting system was tested under multiple load conditions: 0 kg, 130 kg, 200 kg, and 270 kg. The measured lifting time remained highly consistent, ranging from 3.10 s to 3.14 s, showing stable lifting performance across increasing payloads.

The final system also showed improvements in maneuverability, collision control, electrical stability, and motor control responsiveness. According to the thesis results, the lifting system met the initial project requirements for reliability, stability, and safety.

## Tech Stack

- ROS / ROS Noetic
- Gazebo simulation
- LiDAR-based mapping and navigation
- MATLAB / Simulink for system identification and PID tuning
- PLC GX Works2 for lift control logic
- ATmega2560 embedded controller
- Autodesk Inventor 2024 for mechanical design and stress analysis

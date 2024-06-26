# SRVEDCA-Projet-Electronique

SRVEDCA means Système de Reconnaissance Vocale et Empreinte Digitale pour le Controle d'Accès. This repository will gather our project source code for subject Interfacing and Electronics

# To be applied

- Arduino programming skills learned in this course
- Internet Of Things (IOT) learned in this course with Mr David B.
- Human Machine Interaction (HMI) skills in the presentation logic
- API programming learned in web programming course

# Figma Dashboard design
- Check: https://www.figma.com/team_invite/redeem/TNdjdgWGaYzxtBlaqD77eR

# Interface View After Design and Implementation
![Capture_d_écran_2023-04-05_104018-removebg-preview](https://user-images.githubusercontent.com/99027624/230046623-d834a8ee-2194-4cbd-ae03-3d6c59242a53.png)
![Capture_d_écran_2023-04-05_113754-removebg-preview](https://user-images.githubusercontent.com/99027624/230057653-a57e75f9-6ce5-411a-86e2-f0bc9f490b9c.png)
![Frame 1000000917](https://user-images.githubusercontent.com/99027624/230062596-3c5eac49-dd7f-4a2c-91fc-19723838d9d3.png)

# List of equipments
- Arduino Mega
- Voice recognition module
- Fingerprint module
- RTC (Real Time Clock) Module
- PIR module
- Ultrasonic sensor
- Servo Motor
- Electric door Lock
- TFT touch screen
- ESP32

# Repository structure and description

|____ Arduino Programming (Each component necessary library and code are stored)

    |____Ecran TFT
    |____Electric Door Lock
    |____Fingerprint Module
    |____Module RTC
    |____Motion Sensor (PIR)
    |____Servo Motor
    |____Voice Recognition Module

|____ Backend (Servers and the database or fonctional logic are stored)

    |____API (NodeJS, express)
    |____Database (MongoDB)
    |____ESP32 Server

|____ Frontend (Done in ReactJS, All the presentation logic)

    |____srvedca-frontend (React Folder Starter Code)
            |____ public  (The first page of the project)
            |____ src
                    |____ assets
                            |____ css (external bootstrap or styling files)
                            |____ Images (useful Images)
                            |____ webfonts (external google fonts)
                    |____ Components (Contains all reusable components)
                            |____ Button (Default Component)
                                    |____ Button.css (Button CSS Styles)
                                    |____ Button.jsx (React Class Component)
                            |____ NavBar (Navigation bar Component)
                                    |____ NavBar.css (NavBar CSS Styles)
                                    |____ NavBar.jsx (React Class Component)
                    |____ Routes (Contains all dashboard pages)
                            |____ HomePage (Default Page)
                                    |____ HomePage.jsx (React Class Component)
                                    |____ HomePage.css (Home Page CSS Styles)
                    |____ App.css (Extra css styling)
                    |____ App.js (Link all routing pages)
                    |____ App.test.js (Where deployment tests are perform)
                    |____ index.js (Links the App.js Component to the div with id "root")
                    |____ index.css (First page css styles)
            |____ package-lock.json
            |____ package.json
    |____Coleur de theme pour le frontend.jpeg (Frontend Color Theme)

|____ Plannification des taches pour les congés.pdf

|____ README.md

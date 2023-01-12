import React, { useState, useEffect } from 'react'
import './HomeControls.css'

import {HomeControlButton} from "../../Components"
import EnrolStudents from "../../assets/Images/EnrolStudents.png"
import Class from "../../assets/Images/Class.png"
import Courses from "../../assets/Images/Courses.png"
import Admin from "../../assets/Images/Admin.png"
import axios from 'axios'


const HomeControls =  () => {
    const [nbreAdmin, setNbreAdmin] = useState(0);

    const updateTab= ()=>{
        const url = "http://localhost:8080/api/admin/all";
        axios.get(url).then((response) => {
          localStorage.setItem("nbreAdmin", response.data.length);
          setNbreAdmin(response.data.length)
        }).catch(error => {
          console.log(error)
        })
    };
    //if(localStorage.getItem("nbreAdmin")) setNbreAdmin(localStorage.getItem("nbreAdmin"));

    useEffect(() => {
        updateTab()
        if(localStorage.getItem("nbreAdmin")) setNbreAdmin(localStorage.getItem("nbreAdmin"));
      }, [localStorage.getItem("nbreAdmin")]);

    const controlsLayout = [
        {
            bgColor: {backgroundImage: "linear-gradient(130deg, #C7E2EB, #2A9ADA)"},
            icon: EnrolStudents,
            label: "Etudiants Enrôlés",
            registeredNumber: 10
        },
        {
            bgColor: {backgroundImage: "linear-gradient(130deg, #A64AEE, #2A9ADA)"},
            icon: Class,
            label: "Classes",
            registeredNumber: 4
        },
        {
            bgColor: {backgroundImage: "linear-gradient(130deg, #603FE3, #29ABF4)"},
            icon: Courses,
            label: "Cours",
            registeredNumber: 12
        },
        {
            bgColor: {backgroundImage: "linear-gradient(130deg, #603FE3, #86BAFC)"},
            icon: Admin,
            label: "Administrateurs",
            registeredNumber: nbreAdmin
        }
    ]

    const ControlsButton = controlsLayout.map((layout) =>
    <HomeControlButton
        bgColor = {layout.bgColor}
        icon = {layout.icon}
        label = {layout.label}
        registeredNumber = {layout.registeredNumber}
    />)
    
    return (
        <div className='homeControls-container'>
            {ControlsButton}
        </div>
    )
  
}

export default HomeControls
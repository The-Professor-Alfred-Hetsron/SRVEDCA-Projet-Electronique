import React, { Component } from 'react'
import './MainEnrolmentNotification.css'
import { NavLink } from 'react-router-dom'

import {EnrolementCard} from '../../Components'

export class MainEnrolmentNotification extends Component {
  render() {
    const enrolledInfo = [
      {
        id: 1,
        class: "4GI",
        color: {backgroundColor: "#A64AEE"},
        name: "KEGNE MYRIAM ARMELLE",
        matricule: "19P001",
        date:"29 Nov, 2022"
      },
      {
        id: 2,
        class: "4GC",
        color: {backgroundColor: "#F69A36"},
        name: "YEMKWA EMMANUEL",
        matricule: "19P003",
        date:"02 Dec, 2022"
      },
      {
        id: 3,
        class: "4GELE",
        color: {backgroundColor: "#453CC9"},
        name: "DJOUMESSI AUREL",
        matricule: "21P001",
        date:"05 Dec, 2022"
      },
      {
        id: 4,
        class: "5GM",
        color: {backgroundColor: "#EC5555"},
        name: "EWOKI VALERIE",
        matricule: "18P001",
        date:"06 Dec, 2022"
      },
      {
        id: 5,
        class: "MSP2",
        color: {backgroundColor: "#FFCD4D"},
        name: "MENRA ROMIAL",
        matricule: "21P054",
        date:"12 Dec, 2022"
      }
    ]

    const enrolledstudents = enrolledInfo.map((info) => 
    <EnrolementCard
      key={info.id}
      class = {info.class}
      color = {info.color}
      name = {info.name}
      matricule = {info.matricule}
      date = {info.date}
    />)
    return (
      <div className='Notification-container'>
        <div className="Notification-container-div">
          <div className="notif-enrol-number-div">
            <span className="notif-enrol">Enrôlement</span>
            <span className="notif-number">7</span>
          </div>
          <NavLink to='/etudiant' className="notif-viewall">Tout Regarder</NavLink>
        </div>
        {enrolledstudents}
      </div>
    )
  }
}

export default MainEnrolmentNotification
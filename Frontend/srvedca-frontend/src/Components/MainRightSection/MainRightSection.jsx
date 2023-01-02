import React, { Component } from 'react'
import './MainRightSection.css'

import {Calendar} from "../../Components"
import {MainEnrolmentNotification} from "../../Components"

export class MainRightSection extends Component {
  render() {
    return (
      <div className='mainRightSection-container'>
        <Calendar/>
        <MainEnrolmentNotification/>
      </div>
    )
  }
}

export default MainRightSection
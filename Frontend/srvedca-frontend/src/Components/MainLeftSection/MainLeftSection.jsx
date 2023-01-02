import React, { Component } from 'react'
import './MainLeftSection.css'

import {MainWelcomeHeader} from '../../Components'
import {HomeControls} from '../../Components'

export class MainLeftSection extends Component {
  render() {
    return (
      <div className='mainLeftSection-container'>
        <MainWelcomeHeader/>
        <HomeControls/>
      </div>
    )
  }
}

export default MainLeftSection
import React, { Component } from 'react'
import './MainBody.css'

import {MainLeftSection} from '..'
import {MainRightSection} from '..'

export class MainBody extends Component {
  render() {
    return (
        <div className='mainBody-container'>
            <MainLeftSection/>
            <MainRightSection/>
        </div>
    )
  }
}

export default MainBody
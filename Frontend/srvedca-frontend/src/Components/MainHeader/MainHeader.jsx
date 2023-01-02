import React, { Component } from 'react'
import "./MainHeader.css"

import {ProfileHeader} from '..'
import {PageNameHeader} from '..'

export class MainHeader extends Component {
  render() {
    return (
      <div className='mainTopHeader-container'>
        <ProfileHeader/>
        <PageNameHeader/>
      </div>
    )
  }
}

export default MainHeader
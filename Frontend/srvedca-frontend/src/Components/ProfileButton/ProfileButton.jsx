import React, { Component } from 'react'
import './ProfileButton.css'

import profileFace from '../../assets/Images/profileFace.jpg'

export class ProfileButton extends Component {
  render() {
    return (
      <div className='profileButton-container'>
        <img src={profileFace} alt="" className='profileButton-container-img'/>
        <span className='profileButton-container-span'>Vicky Garba</span>
      </div>
    )
  }
}

export default ProfileButton
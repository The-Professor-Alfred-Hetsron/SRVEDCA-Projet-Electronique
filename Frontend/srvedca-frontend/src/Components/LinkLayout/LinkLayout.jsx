import React, { Component } from 'react'
import './LinkLayout.css'

import { NavLink } from 'react-router-dom'
import { ReactComponent as DashboardHomeLink } from '../../assets/Images/DashboardHome_Link.svg'

export class LinkLayout extends Component {
  render() {

    return (
        <NavLink className='linklayout-container' to='/'>
            <DashboardHomeLink fill='black'/>
            <span>Accueil</span>
        </NavLink>
    )
  }
}

export default LinkLayout
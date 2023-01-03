import React, { Component } from 'react'
import './LinkLayout.css'

import { NavLink } from 'react-router-dom'
import { ReactComponent as DashboardHomeLink } from '../../assets/Images/DashboardHome_Link.svg'

export class LinkLayout extends Component {
  render() {
    const myLink = {
        fill: 'white',
        ':hover': {
            fill: '#22AFF1'
        },
        ':active': {
            fill: 'black'
        }
    }
const navLinkStyles = ({isActive}) =>{
    return{
        color: isActive ? 'black' : 'white',
        backgroundColor: isActive ? 'white' : 'transparent'
    }
}

    return (
        <NavLink className='linklayout-container' to='/'>
            <DashboardHomeLink fill='white'/>
            <DashboardHomeLink fill='#22AFF1'/>
            <DashboardHomeLink fill='black'/>
            <span>LinkLayout</span>
        </NavLink>
    )
  }
}

export default LinkLayout
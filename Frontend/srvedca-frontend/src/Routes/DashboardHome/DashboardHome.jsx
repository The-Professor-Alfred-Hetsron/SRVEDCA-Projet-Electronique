import React, { Component } from 'react'
import './DashboardHome.css'

import {PageBody} from "../../Components"
import {NavBar} from '../../Components'

export class DashboardHome extends Component {
    render() {
        return (
            <div className='dashboard-home-container'>
                <NavBar/>
                <PageBody/>
            </div>
            
        )
    }
}

export default DashboardHome
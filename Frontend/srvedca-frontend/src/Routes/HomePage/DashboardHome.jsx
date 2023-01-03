import React, { Component } from 'react'
import './DashboardHome.css'

import {PageBody} from "../../Components"
import {NavBar} from '../../Components'

export class DashboardHome extends Component {
    render() {
        const pageType = "accueil"
        return (
            <div className='dashboard-container'>
                <NavBar page={pageType}/>
                <PageBody page={pageType}/>
            </div>
            
        )
    }
}

export default DashboardHome
import React, { Component } from 'react'
import './DashboardPlanning.css'

import {PageBody} from "../../Components"
import {NavBar} from '../../Components'

export class DashboardPlanning extends Component {
    
    render() {
        const pageType = "planning"
        return (
            <div className='dashboard-container'>
                <NavBar page={pageType}/>
                <PageBody page={pageType}/>
            </div>
            
        )
    }
}

export default DashboardPlanning
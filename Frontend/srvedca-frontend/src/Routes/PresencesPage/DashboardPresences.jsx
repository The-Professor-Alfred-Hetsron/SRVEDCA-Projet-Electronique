import React, { Component } from 'react'
import './DashboardPresences.css'

import {PageBody} from "../../Components"
import {NavBar} from '../../Components'

export class DashboardPresences extends Component {
    
    render() {
        const pageType = "présence"
        return (
            <div className='dashboard-container'>
                <NavBar page={pageType}/>
                <PageBody page={pageType}/>
            </div>
            
        )
    }
}

export default DashboardPresences
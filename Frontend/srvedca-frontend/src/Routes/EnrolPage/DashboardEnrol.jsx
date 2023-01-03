import React, { Component } from 'react'
import './DashboardEnrol.css'

import {PageBody} from "../../Components"
import {NavBar} from '../../Components'

export class DashboardEnrol extends Component {
    
    render() {
        const pageType = "etudiant"
        return (
            <div className='dashboard-container'>
                <NavBar page={pageType}/>
                <PageBody page={pageType}/>
            </div>
            
        )
    }
}

export default DashboardEnrol
import React, { Component } from 'react'
import './DashboardCourse.css'

import {PageBody} from "../../Components"
import {NavBar} from '../../Components'

export class DashboardCourse extends Component {
    
    render() {
        const pageType = "cours"
        return (
            <div className='dashboard-container'>
                <NavBar page={pageType}/>
                <PageBody page={pageType}/>
            </div>
            
        )
    }
}

export default DashboardCourse
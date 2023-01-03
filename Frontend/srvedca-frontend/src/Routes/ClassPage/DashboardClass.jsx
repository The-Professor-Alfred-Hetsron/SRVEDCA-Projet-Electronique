import React, { Component } from 'react'
import './DashboardClass.css'

import {PageBody} from "../../Components"
import {NavBar} from '../../Components'

export class DashboardClass extends Component {
    
    render() {
        const pageType = "classe"
        return (
            <div className='dashboard-container'>
                <NavBar page={pageType}/>
                <PageBody page={pageType}/>
            </div>
            
        )
    }
}

export default DashboardClass
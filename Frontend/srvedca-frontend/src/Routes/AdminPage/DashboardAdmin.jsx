import React, { Component } from 'react'
import './DashboardAdmin.css'

import {PageBody} from "../../Components"
import {NavBar} from '../../Components'

export class DashboardAdmin extends Component {
    
    render() {
        const pageType = "admin"
        return (
            <div className='dashboard-container'>
                <NavBar page={pageType}/>
                <PageBody page={pageType}/>
            </div>
            
        )
    }
}

export default DashboardAdmin
import React, { Component } from 'react'
import './NavBar.css'

import { NavLink } from 'react-router-dom'
import HomeMainTextContent from '../../assets/Images/HomeMainTextContent.svg'

import { ReactComponent as HomeIcon } from '../../assets/Images/Icon1.svg'
import { ReactComponent as EnrolIcon } from '../../assets/Images/Icon2.svg'
import { ReactComponent as CourseIcon } from '../../assets/Images/Icon3.svg'
import { ReactComponent as ClassIcon } from '../../assets/Images/Icon4.svg'
import { ReactComponent as AdminIcon } from '../../assets/Images/Icon7.svg'
import { ReactComponent as PlanningIcon } from '../../assets/Images/Icon6.svg'
import { ReactComponent as PresenceIcon } from '../../assets/Images/Icon5.svg'


export class NavBar extends Component {
    constructor(){
        super()
        this.state = {
            currentLink: 1
        }
    }
    setCurrentLink = (value) =>{
        this.setState({
            currentLink: value
        })
    }
    render() {
        const allLinks = [
            {
                id: 1,
                label: "Accueil",
                path: "/accueil",
                icon: <HomeIcon/>,
            },
            {
                id: 2,
                label: "Etudiants Enrolés",
                path: "/etudiant",
                icon: <EnrolIcon/>,
            },
            {
                id: 3,
                label: "Classes",
                path: "/classe",
                icon: <ClassIcon/>,
            },
            {
                id: 4,
                label: "Cours",
                path: "/cours",
                icon: <CourseIcon/>,
            },
            {
                id: 5,
                label: "Administrateurs",
                path: "/admin",
                icon: <AdminIcon/>,
            },
            {
                id: 6,
                label: "Planning de cours",
                path: "/planning",
                icon: <PlanningIcon/>,
            },
            {
                id: 7,
                label: "Présences",
                path: "/presence",
                icon: <PresenceIcon/>,
            }
        ]

        const NavLinks = allLinks.map((linkInfo) => 
        <li onClick={() => this.setCurrentLink(linkInfo.id)}>
            <NavLink className={this.state.currentLink === linkInfo.id ? "active" : "linklayout-container"} to={linkInfo.path} end>
                {linkInfo.icon}
                <span>{linkInfo.label}</span>
            </NavLink>
        </li>)
        return(
            <nav className="navigation-container">
                <img src={HomeMainTextContent} alt="" className="navigation-container-img" />
                <ul className="navigation-list">
                    {NavLinks}
                </ul>

                {/*<nav>
            <NavLink to='/accueil'>Accueil</NavLink>
            <NavLink to='/etudiant'>Etudiants Enrôlés</NavLink>
            <NavLink to='/classe'>Classes</NavLink>
            <NavLink to='/cours'>Cours</NavLink>
            <NavLink to='/admin'>Administrateurs</NavLink>
            <NavLink to='/planning'>Planning des cours</NavLink>
        <NavLink to='/presence'>Présences</NavLink>
        </nav>*/}
            </nav>
            
        )
    }
}

export default NavBar
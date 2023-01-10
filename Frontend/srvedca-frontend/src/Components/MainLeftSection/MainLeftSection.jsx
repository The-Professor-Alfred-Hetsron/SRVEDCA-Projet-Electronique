import React, { Component } from 'react'
import './MainLeftSection.css'

import {MainWelcomeHeader} from '../../Components'
import {HomeControls} from '../../Components'
import {TableEnrolment} from '../../Components'
import {TableAdmin} from '../../Components'
import {Courses} from '../../Components'
import {ClassTable, PlanningTable} from '../../Components'

export class MainLeftSection extends Component {
  render() {
    const pageType = this.props.page
    console.log(pageType)
    return (
      <div className='mainLeftSection-container'>
        {pageType === "accueil" ? /*La page Accueil du Dashboard*/
        (
          <div className="mainLeftSection-container-div">
            <MainWelcomeHeader/>
            <HomeControls/>
          </div>
        ) 
        : null}
        {pageType === "etudiant" ?  /*La page Etudiant Enrolés du Dashboard*/
        (
          <div className="mainLeftSection-container-div">
            {/*Ajoute le composant de ta table ici*/}
            <TableEnrolment/>
          </div>
        ) 
        : null}
        {pageType === "classe" ?  /*La page Classes du Dashboard*/
        (
          <div className="mainLeftSection-container-div">
            {/*Ajoute le composant de ta table ici*/}
            <ClassTable />
          </div>
        ) 
        : null}
        {pageType === "cours" ?  /*La page Cours du Dashboard*/
        (
          <div className="mainLeftSection-container-div">
            {/*Ajoute le composant de ta table ici*/}
            <Courses/>
          </div>
        ) 
        : null}
        {pageType === "admin" ?  /*La page Administrateur du Dashboard*/
        (
          <div className="mainLeftSection-container-div">
            {/*Ajoute le composant de ta table ici*/}
            <TableAdmin />
          </div>
        ) 
        : null}
        {pageType === "planning" ?  /*La page Planning de Cours du Dashboard*/
        (
          <div className="mainLeftSection-container-div">
            {/*Ajoute le composant de ta table ici*/}
            <PlanningTable />
          </div>
        ) 
        : null}
        {pageType === "présence" ?  /*La page Présences du Dashboard*/
        (
          <div className="mainLeftSection-container-div">
            {/*Ajoute le composant de ta table ici*/}
            Ajoute le composant de ta table Présences ici
          </div>
        ) 
        : null}
      </div>
    )
  }
}

export default MainLeftSection
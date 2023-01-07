import React, { Component } from 'react';
import './Courses.css';
import Validate from "../../assets/Images/Validate.svg"
import Sprint from "../../assets/Images/Sprint.svg"
import Empreinte from "../../assets/Images/Empreinte.png"
import Search from "../../assets/Images/search.svg"
import Delete from "../../assets/Images/Delete.svg"
import Modifier from "../../assets/Images/Modifier.svg"

export class Courses extends Component {
    render(){

        const Courses = [
            {
                id:1,
                code: "uhezifyzefjkn",
                nom: "Programmation web",
                classe: "4GI",
                semestre:"1",
                date:"02/11/2022",
                statut:"Refusé"
            },
            {
                id:2,
                code: "qjysiqkdulkjs",
                nom: "Microcontrolleur",
                classe: "5GI",
                semestre:"2",
                date:"02/11/2022",
                statut:"Validé"
            },
            {
                id:3,
                code: "qyisicuhqz",
                nom: "IHM",
                classe: "4GI",
                semestre:"1",
                date:"02/11/2022",
                statut:"Validé"
            },
            {
                id:4,
                code: "ksuhfusdksc",
                nom: "Management",
                classe: "5GI",
                semestre:"2",
                date:"14/04/2022",
                statut:"Refusé"
            },
            {
                id:5,
                code: "wjndcsjnjjs",
                nom: "POO2",
                classe: "3GI",
                semestre:"2",
                date:"20/12/2021",
                statut:"Validé"
            }
        ]

        return(
            <div className='Courses-div'>
                <div className='cours-div'>
                    <a className='validate-all'><img src={Validate} alt="validate" /> Valider tout</a>
                    <a className='sprint'><img src={Sprint} alt="sprint" /> Imprimer</a>
                    <form action="" className='form-search'>
                        <div className='input-btn'>
                        <img className='search-icon' src={Search} alt="search" />
                        <input className='search-input' type="text" placeholder='Rechercher Cours...' />
                        </div>
                        <button type='submit' className='search'>Rechercher</button>
                    </form>
                    <table className='Table-course'>
                        <thead>
                            <tr>
                                <th>Id</th>
                                <th>Code</th>
                                <th>Nom</th>
                                <th>Classe</th>
                                <th>Semestre</th>
                                <th>Date</th>
                                <th>Statut</th>
                                <th>Actions</th>
                            </tr>
                        </thead>
                        <tbody/>
                        {Courses.map((cours, index) =>
                            <tr key={cours.id}>
                                <td>{index + 1}</td>
                                <td>{cours.code}</td>
                                <td className='course-name'>{cours.nom}</td>
                                <td>{cours.classe}</td>
                                <td>{cours.semestre}</td>
                                <td>{cours.date}</td>
                                <td><span className={cours.statut === "Validé" ? "valide" : "Refuse"}>{cours.statut}</span></td>
                                <td>
                                    <button className='button-modifier'><img src={Modifier} alt="modifier" />Modifier</button>
                                    <button className='button-sup'><img src={Delete} alt="delete" />Supprimer</button>
                                </td>
                            </tr>
                        )}
                        <tbody/>
                    </table>
                    
            
                </div>
                <div className='carroussel-btn-course'>
                    <div className='precedent'>Précèdent</div>
                    <div className='page-number'>
                        <span className='previous-page'>1</span>
                        <span className='next-page'>2</span></div>
                    <div className='suivant'>Suivant</div>
                </div>
            </div>
        );
    }

}

export default Courses
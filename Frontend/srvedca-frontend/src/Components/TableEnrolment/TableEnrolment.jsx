import React, { Component } from 'react';
import './TableEnrolment.css';
import Validate from "../../assets/Images/Validate.svg"
import Sprint from "../../assets/Images/Sprint.svg"
import Empreinte from "../../assets/Images/Empreinte.png"
import Search from "../../assets/Images/search.svg"
import Delete from "../../assets/Images/Delete.svg"
import Modifier from "../../assets/Images/Modifier.svg"

export class TableEnrolment extends Component {
    render(){

        const Students = [
            {
                id: 1,
                matricule: "19P024",
                nom: "Alfred Alex Jude",
                classe: "4GI",
                empreinte:"5C 3A A4 5B 24 56 18 4D AA FB 5F 7F BB AC 5D F4 AF 78 98 2A 5C 2E EE A4 5F 5C CD AD AB 6F 4E 2B",
                imageEmpreinte: Empreinte,
                date:"02/11/2022",
                statut:"Validé"
            },
            {
                id: 2,
                matricule: "19P019",
                nom: "Ewoki ebouele ange",
                class: "5GI",
                empreinte:"5C 3A A4 5B 24 56 18 4D AA FB 5F 7F BB AC 5D F4 AF 78 98 2A 5C 2E EE A4 5F 5C CD AD AB 6F 4E 2B",
                imageEmpreinte: Empreinte,
                date:"21/11/2022",
                statut:"Validé"
            },
            {
                id: 3,
                matricule: "19P002",
                nom: "Vicky Garba",
                class: "5GI",
                empreinte:"5C 3A A4 5B 24 56 18 4D AA FB 5F 7F BB AC 5D F4 AF 78 98 2A 5C 2E EE A4 5F 5C CD AD AB 6F 4E 2B",
                imageEmpreinte: Empreinte,
                date:"16/11/2022",
                statut:"Refusé"
            }
        ]

        return(
            <div className='students-div'>
                <div className='TableEnrol-div'>

            
                
                    <a className='validate-all'><img src={Validate} alt="validate" /> Valider tout</a>
                    <a className='sprint'><img src={Sprint} alt="sprint" /> Imprimer</a>
                    <form action="" className='form-search'>
                        <div className='input-btn'>
                        <img className='search-icon' src={Search} alt="search" />
                        <input className='search-input' type="text" placeholder='Rechercher Etudiant...' />
                        </div>
                        <button type='submit' className='search'>Rechercher</button>
                    </form>
                    <table className='Table'>
                        <thead>
                            <tr>
                                <th>Id</th>
                                <th>Matricule</th>
                                <th>Noms</th>
                                <th>Classes</th>
                                <th>Empreinte digitale</th>
                                <th>Image empreinte</th>
                                <th>Date</th>
                                <th>Statut</th>
                                <th>Actions</th>
                            </tr>
                        </thead>
                        <tbody/>
                        {Students.map((student, index) =>
                            <tr key={student.id}>
                                <td>{index + 1}</td>
                                <td>{student.matricule}</td>
                                <td>{student.nom}</td>
                                <td>{student.class}</td>
                                <td>{student.empreinte}</td>
                                <td><img src={student.imageEmpreinte} alt="" /></td>
                                <td>{student.date}</td>
                                <td><span className={student.statut === "Validé" ? "valide" : "Refuse"}>{student.statut}</span></td>
                                <td>
                                    <button className='button-modifier'><img src={Modifier} alt="modifier" />Modifier</button>
                                    <button className='button-sup'><img src={Delete} alt="delete" />Supprimer</button>
                                </td>
                            </tr>
                        )}
                        <tbody/>
                    </table>
                </div>
                <div className='carroussel-btn'>
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

export default TableEnrolment
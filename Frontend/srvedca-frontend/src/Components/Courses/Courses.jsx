import React, { useState, useRef } from 'react';
import './Courses.css';
import Validate from "../../assets/Images/Validate.svg"
import Sprint from "../../assets/Images/Sprint.svg"
import Empreinte from "../../assets/Images/Empreinte.png"
import Search from "../../assets/Images/search.svg"
import Delete from "../../assets/Images/Delete.svg"
import Modifier from "../../assets/Images/Modifier.svg"
import Modal from 'react-modal';
import ReactJsAlert from "reactjs-alert";
import { GrFormClose } from "react-icons/gr";

const Courses = () => {

         //Modal 1
         const [modalIsOpens1, setIsOpens1] = useState(false);
         let subtitle1;
 
         function openModal1() {
             setIsOpens1(true);
         }
 
 
         function closeModal1() {
             setIsOpens1(false);
         }
 
         const customStyles1 = {
             content: {
               top: '40%',
               left: '50%',
               right: 'auto',
               bottom: 'auto',
               marginRight: '-50%',
               padding: '10px 50px',
               transform: 'translate(-50%, -50%)',
             },
           };
     
           
           //fin modal 1
 
           //Modal 2
         const [modalIsOpens, setIsOpens] = useState(false);
 
         //Alert 
 
         const [status, setStatus] = useState(false);
         const [type, setType] = useState("");
         const [title, setTitle] = useState("");
 
         function openModal() {
             setIsOpens(true);
         }
 
         function afterOpenModal() {
             // references are now sync'd and can be accessed.
             subtitle.style.color = '#000';
         }
 
         function closeModal() {
             setIsOpens(false);
         }
 
         const customStyles = {
             content: {
               top: '40%',
               left: '50%',
               right: 'auto',
               bottom: 'auto',
               marginRight: '-50%',
               padding: '10px 50px',
               transform: 'translate(-50%, -50%)',
             },
           };
     
           let subtitle;
           //fin modal 2
 
     const [name, setName] = useState("");
     const [semestre, setSemestre] = useState("");
     const [code, setCode] = useState("");
     const [classe, setClasse] = useState("");
     const formRef = useRef()

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
                    <a className='ajout'>Ajouter</a>
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
                                <td>
                                    <button onClick={openModal} className='button-modifier'><img src={Modifier} alt="modifier" />Modifier</button>
                                    <button onClick={openModal1} className='button-sup'><img src={Delete} alt="delete" />Supprimer</button>
                                </td>
                            </tr>
                        )}
                        <tbody/>
                    </table>
                </div>
                <div>
                <Modal
                    isOpen={modalIsOpens}
                    onAfterOpen={afterOpenModal}
                    onRequestClose={closeModal}
                    style={customStyles}
                    contentLabel="Example Modal"
                >
                  <a onClick={closeModal} className="close-btn-modal"><GrFormClose /></a>
                    <h2 ref={(_subtitle) => (subtitle = _subtitle)}>Modifier l'étudiant</h2>
                    
                    <form ref={formRef}  className="form-class">
                <label className="labelForm" htmlFor="code">Entrer le code</label>
                <input className="label-input" id="code" type="text" placeholder="Code..." name="matricule" value={code} onChange = {(e) => setCode(e.target.value)} required/>
                <label className="labelForm" htmlFor="name">Entrer le nom</label>
                <input className="label-input" id="name" type="text" placeholder="Nom..." name="name" value={name} onChange = {(e) => setName(e.target.value)} required/>
                <label className="labelForm" htmlFor="semestre">Entrer le Semestre</label>
                <input className="label-input" id="semestre" type="text" placeholder="Semestre..." name="semestre" value={semestre} onChange = {(e) => setSemestre(e.target.value)} required/>
                <label className="labelForm" htmlFor="class">Entrer sa nouvelle classe</label>
                <input className="label-input" id="class" type="text" placeholder="Classe..." name="classe" value={classe} onChange = {(e) => setClasse(e.target.value)} required/>
                        
                <ReactJsAlert
                    status={status} // true or false
                    type={type} // success, warning, error, info
                    title={title}
                    quotes={true}
                    quote=""
                    Close={() => setStatus(false)}
                />
            <button type='button' className='btn-btn-primary' style={{borderRadius:5,textAlign:'center', padding:10,color:'white',background:'#939af0'}}>Ajouter</button>

        </form>
                </Modal>
            </div>
            <div>
                <Modal
                    isOpen={modalIsOpens1}
                    onRequestClose={closeModal1}
                    style={customStyles1}
                    contentLabel="Example Modal"
                >
                  <a onClick={closeModal1} className="close-btn-modal"><GrFormClose /></a>
                    <h2 ref={(_subtitle) => (subtitle = _subtitle)}>Voulez vous vraiment supprimer?</h2>
                    
                            <div className='Validate-action'>
                                <a className='Validate-oui' href="">Oui</a>
                                <a className='Validate-non' href="">Non</a>
                            </div>

                </Modal>
            </div>
            </div>
        );
    }

export default Courses
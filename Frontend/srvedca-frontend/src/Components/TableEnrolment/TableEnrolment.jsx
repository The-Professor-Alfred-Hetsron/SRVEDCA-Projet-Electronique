import React, { useRef,useState } from 'react';
import './TableEnrolment.css';
import Validate from "../../assets/Images/Validate.svg"
import Sprint from "../../assets/Images/Sprint.svg"
import Empreinte from "../../assets/Images/Empreinte.png"
import Search from "../../assets/Images/search.svg"
import Delete from "../../assets/Images/Delete.svg"
import Modifier from "../../assets/Images/Modifier.svg"
import Modal from 'react-modal';
import ReactJsAlert from "reactjs-alert";
import { GrFormClose } from "react-icons/gr";

const TableEnrolment = () => {

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
    const [mat, setMat] = useState("");
    const [classe, setClasse] = useState("");
    const formRef = useRef()

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
                statut:"En attente"
            },
            {
                id: 3,
                matricule: "19P002",
                nom: "Vicky Garba",
                class: "5GI",
                empreinte:"5C 3A A4 5B 24 56 18 4D AA FB 5F 7F BB AC 5D F4 AF 78 98 2A 5C 2E EE A4 5F 5C CD AD AB 6F 4E 2B",
                imageEmpreinte: Empreinte,
                date:"16/11/2022",
                statut:"En attente"
            },
            {
                id: 4,
                matricule: "19P002",
                nom: "Vicky Garba",
                class: "5GI",
                empreinte:"5C 3A A4 5B 24 56 18 4D AA FB 5F 7F BB AC 5D F4 AF 78 98 2A 5C 2E EE A4 5F 5C CD AD AB 6F 4E 2B",
                imageEmpreinte: Empreinte,
                date:"16/11/2022",
                statut:"Validé"
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
                                <td className='student-name'>{student.nom}</td>
                                <td>{student.class}</td>
                                <td>{student.empreinte}</td>
                                <td><img src={student.imageEmpreinte} alt="" /></td>
                                <td>{student.date}</td>
                                <td><span className={student.statut === "Validé" ? "valide" : "attente"}>{student.statut}</span></td>
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
          
                <label className="labelForm" htmlFor="name">Entrer le nouveau nom</label>
                <input className="label-input" id="name" type="text" placeholder="Nouveau nom..." name="name" value={name} onChange = {(e) => setName(e.target.value)} required/>
                <label className="labelForm" htmlFor="mat">Entrer le nouveau matricule</label>
                <input className="label-input" id="mat" type="text" placeholder="Nouveau matricule..." name="matricule" value={mat} onChange = {(e) => setMat(e.target.value)} required/>
                <label className="labelForm" htmlFor="class">Entrer sa nouvelle classe</label>
                <input className="label-input" id="class" type="text" placeholder="Nouvelle classe..." name="classe" value={classe} onChange = {(e) => setClasse(e.target.value)} required/>
                        
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

export default TableEnrolment
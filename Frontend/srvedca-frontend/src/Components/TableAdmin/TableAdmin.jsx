import React, { useState,useRef } from 'react';
import './TableAdmin.css';
import Validate from "../../assets/Images/Validate.svg"
import Sprint from "../../assets/Images/Sprint.svg"
import Search from "../../assets/Images/search.svg"
import Delete from "../../assets/Images/Delete.svg"
import Modifier from "../../assets/Images/Modifier.svg"
import Modal from 'react-modal';
import ReactJsAlert from "reactjs-alert";
import { GrFormClose } from "react-icons/gr";

const TableAdmin = () => {

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
const [email, setEmail] = useState("");
const [tel, setTel] = useState("");
const formRef = useRef()

        const TableAdmins = [
            {
                id:1,
                nom: "CHANA LEMALE",
                email: "hdbjsfj@gmail.com",
                tel:"59989809",
                motdepasse:"kshdksjnkdfskdfsk",
                role:"superadmin",
                statut:"Validé"
            },
            {
                id:2,
                nom: "DJOUMESSI AUREL",
                email: "utdqdqs@gmail.com",
                tel:"675767676",
                motdepasse:"kshdksjnkdfskdfsk",
                role:"admin",
                statut:"Validé"
                
            },
            {
                id:2,
                nom: "Ewoki Ebouele",
                email: "eojsnfs@gmail.com",
                tel:"798447676",
                motdepasse:"kshdksjnkdfskdfsk",
                role:"admin",
                statut:"Refusé"
            },
            {
                id:3,
                nom: "ALFRED HERTSON",
                email: "alfred@gmail.com",
                tel:"12I3234099",
                motdepasse:"kshdksjnkdfskdfsk",
                role:"admin",
                statut:"Validé"
            },
            {
                id:4,
                nom: "ZIINAM VICKY GARBA",
                email: "nkfjshufr@gmail.com",
                tel:"989897",
                motdepasse:"kshdksjnkdfskdfsk",
                role:"admin",
                statut:"Refusé"
            },
        ]

        return(
            <div className='TableAdmin-div'>
                <div className='tableAdmin-div'>
                    <a className='validate-all'><img src={Validate} alt="validate" /> Valider tout</a>
                    <a className='sprint'><img src={Sprint} alt="sprint" /> Imprimer</a>
                    <form action="" className='form-search'>
                        <div className='input-btn'>
                        <img className='search-icon' src={Search} alt="search" />
                        <input className='search-input' type="text" placeholder='Rechercher Admin...' />
                        </div>
                        <button type='submit' className='search'>Rechercher</button>
                    </form>
                    <table className='Table-TableAdmin'>
                        <thead>
                            <tr>
                                <th>Id</th>
                                <th>Nom</th>
                                <th>Email</th>
                                <th>Téléphone</th>
                                <th>Role</th>
                                <th>Statut</th>
                                <th>Actions</th>
                            </tr>
                        </thead>
                        <tbody/>
                        {TableAdmins.map((admin, index) =>
                            <tr key={admin.id}>
                                <td>{index + 1}</td>
                                <td className='tableAdmin-name'>{admin.nom}</td>
                                <td>{admin.email}</td>
                                <td>{admin.tel}</td>
                                <td>{admin.role}</td>
                                <td><span className={admin.statut === "Validé" ? "valide" : "Refuse"}>{admin.statut}</span></td>
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
                <input className="label-input" id="name" type="text" placeholder="Nom..." name="name" value={name} onChange = {(e) => setName(e.target.value)} required/>
                <label className="labelForm" htmlFor="email">Entrer l'email</label>
                <input className="label-input" id="email" type="email" placeholder="Email..." name="email" value={email} onChange = {(e) => setEmail(e.target.value)} required/>
                <label className="labelForm" htmlFor="tel">Entrer son numéro de téléphone</label>
                <input className="label-input" id="tel" type="number" placeholder="Téléphone..." name="tel" value={tel} onChange = {(e) => setTel(e.target.value)} required/>
                        
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

export default TableAdmin
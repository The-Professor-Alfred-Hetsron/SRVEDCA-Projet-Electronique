import React, { useRef } from 'react'
import './ClassTable.css'
import Modal from 'react-modal'
import { useState } from 'react'
import {GrFormClose} from 'react-icons/gr'
import ReactJsAlert from 'reactjs-alert'


const classTable = () => {

    const classes = [
        {
            id: 1,
            nom: "4GI",
            salle: "B11"
        },
        {
            id: 2,
            nom: "4GELE",
            salle: "B14"
        }
    ]
    const [modalIsOpens, setIsOpens] = useState(false)

    function openModal() {
        setIsOpens(true);
    }

  

    function closeModal(){
        setIsOpens(false);
    }
    const customStyles = {
        content: {
            top: '40%',
            left: '50%',
            right: 'auto',
            bottom: 'auto',
            marginRight: '-50%',
            transform: 'translate(-50%, -50%)',
        }
    }
    let subtitle;

    const [status, setStatus] = useState(false);
        const [type, setType] = useState("");
        const [title, setTitle] = useState("");
        const formRef = useRef()

    const [modalIsOpens1, setIsOpens1] = useState(false)
    const [salle, setSalle] = useState("")
    const [name, setName] = useState("")

    function openModal1() {
        setIsOpens1(true);
    }

    function afterOpenModal1(){
        subtitle1.style.color = '#000'
    }

    function closeModal1(){
        setIsOpens1(false);
    }
    const customStyles1 = {
        content: {
            top: '40%',
            left: '50%',
            right: 'auto',
            bottom: 'auto',
            marginRight: '-50%',
            transform: 'translate(-50%, -50%)',
        }
    }
    let subtitle1;


    const classTag = classes.map((classe) => <tr>
    <td>{classe.id}</td>
    <td>{classe.nom}</td>
    <td>{classe.salle}</td>
    <td>
        <button onClick={openModal1}>modifier</button> <br></br>
        <button onClick={openModal}>Supprimer</button>
    </td>
    </tr>)

    return (
        <div class="tableau">
            <header>
                <button>Valider</button>
                <button>Imprimer</button>
                <input type="search"/>
                <button>Recherche</button>
            </header>
            <table>
                <thead>
                    <tr>
                       <th>id</th>
                       <th>Noms</th>
                       <th>Salle</th>
                       <th>Actions</th>
                    </tr>    
                </thead>

                <tbody>
                    {classTag}
                </tbody>

            </table>
            <div>
                <Modal
                    isOpen={modalIsOpens}
                    onRequestClose={closeModal}
                    style={customStyles}
                    contentLabel="Example Modal"
                >
                  <a onClick={closeModal} className="close-btn-modal"><GrFormClose /></a>
                  <div className='modal'>
                    <div className='title'>
                        <h2 ref={(_subtitle1) => (subtitle1 = _subtitle1)}>Are you sure you want to continue?</h2>
                    </div>
                    <div className='footer'>
                        <div className='cancel-btn'>
                             <button onClick={() => closeModal(false)}>Cancel</button>
                        </div> 
                        <button>Continue</button>
                     </div>
                  </div>
                    
                </Modal>
                <Modal
                    isOpen={modalIsOpens1}
                    onAfterOpen={afterOpenModal1}
                    onRequestClose={closeModal1}
                    style={customStyles1}
                    contentLabel="Example Modal"
                >
                  <a onClick={closeModal1} className="close-btn-modal"><GrFormClose /></a>
                <h2 ref={(_subtitle) => (subtitle1 = _subtitle)}>Modifier les informations </h2>
                <form ref={formRef}  className="form-class">
          
                    <label className="labelForm" htmlFor="name">Entrer le nouveau nom</label>
                    <input className="label-input" id="name" type="text" placeholder="Nouveau nom..." name="name" value={name} onChange = {(e) => setName(e.target.value)} required/>
                    <label className="labelForm" htmlFor="salle">Entrer la nouvelle salle</label>
                    <input className="label-input" id="salle" type="text" placeholder="Nouvelle Salle..." name="salle" value={salle} onChange = {(e) => setSalle(e.target.value)} required/>
                        
                <ReactJsAlert
                    status={status} // true or false
                    type={type} // success, warning, error, info
                    title={title}
                    quotes={true}
                    quote=""
                    Close={() => setStatus(false)}
                />
                <button type='button' className='btn-btn-primary' style={{borderRadius:5,textAlign:'center', padding:10,color:'white',background:'#939af0'}}>Modifier</button>

                </form>
                 
                    
                </Modal>
            </div>
        </div>
    )

}

export default classTable
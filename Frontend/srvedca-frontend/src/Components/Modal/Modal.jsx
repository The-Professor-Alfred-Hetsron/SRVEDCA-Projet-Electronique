import React from 'react'
import "./Modal.css"

function Modal({closeModal}) {
  return (
    <div className="modalBackground">
        <div className="modalContainer">
                <button onClick={() => closeModal(false)}> X </button>
            <div className='title'>
                <h1>Are you sure You want To Continue</h1>
            </div>
            <div className="body"></div>
            <div className='footer'>
                <button onClick={() => closeModal(false)}> Cancel </button>

                <button>Continue</button>
            </div>

        </div>
    </div>
  )
}

export default Modal
import React, { Component } from 'react'
import './HomeControlButton.css'


export class HomeControlButton extends Component {
  render() {
    
    return (
        <div className='homeControlButton-container' style={this.props.bgColor}>
            <div className='homeControlButton-container-div'>
                <img src={this.props.icon} alt="" className="homeControlButton-container-div-img" />
                <span className='homeControlButton-container-div-span'><span>{this.props.registeredNumber}</span></span>
            </div>
            <span className="homeControlButton-container-span">{this.props.label}</span>
        </div>
    )
  }
}

export default HomeControlButton
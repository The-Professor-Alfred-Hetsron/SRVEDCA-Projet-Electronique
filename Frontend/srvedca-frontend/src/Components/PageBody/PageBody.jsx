import React, { Component } from 'react'
import "./PageBody.css"

import {MainHeader} from '../../Components'
import {MainBody} from '../../Components'

export class PageBody extends Component {
  render() {
    return (
      <div className='pagebody-container'>
        <MainHeader/>
        <MainBody/>
      </div>
    )
  }
}

export default PageBody
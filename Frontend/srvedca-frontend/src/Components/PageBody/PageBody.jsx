import React, { Component } from 'react'
import "./PageBody.css"

import {MainHeader} from '../../Components'
import {MainBody} from '../../Components'

export class PageBody extends Component {
  render() {
    const page = this.props.page
    return (
      <div className='pagebody-container'>
        <MainHeader/>
        <MainBody page={page}/>
      </div>
    )
  }
}

export default PageBody
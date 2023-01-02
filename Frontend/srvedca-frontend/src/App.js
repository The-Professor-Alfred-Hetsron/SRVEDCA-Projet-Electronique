import React from 'react'
import './App.css';
import { DashboardHome } from './Routes';
import { Routes, Route} from 'react-router-dom'


function App() {
  return (
    <div className="App">
      <Routes>
        <Route path='/' element={<DashboardHome/>}/>
      </Routes>
    </div>
  );
}

export default App;

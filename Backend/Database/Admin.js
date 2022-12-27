const mongoose = require('mongoose')

const adminSchema = new mongoose.Schema({
    nom: {
        type: String,
        required: true,
        minLength: 3,
        uppercase: true,
    },
    email: {
        type: String,
        required: true,
        minLength: 6,
        lowercase: true,
        trim: true
    },
    tel: {
        type: String,
    },
    motdepasse: {
        type: String,
        required: true,
    },
    role: {
        type: String,
        enum: ['admin', 'superadmin']
    },
    
}, { 
    timestamps: { createdAt: 'dateCreation', updatedAt: 'dateModification' } 
    })

module.exports = mongoose.model('Admin', adminSchema)

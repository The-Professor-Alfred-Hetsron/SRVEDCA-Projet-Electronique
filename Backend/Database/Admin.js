const mongoose = require('mongoose')
const bcrypt = require('bcrypt')

const adminSchema = new mongoose.Schema({
    nom: {
        type: String,
        required: [true, "Nom requise"],
        minLength: 3,
        uppercase: true,
    },
    email: {
        type: String,
        required: [true,"Email requis"], 
        minLength: 6,
        lowercase: true,
        trim: true
    },
    tel: {
        type: String,
    },
    motdepasse: {
        type: String,
        required: [true,"Mot de passe requis"],
    },
    role: {
        type: String,
        enum: ['admin', 'superadmin']
    },
    
}, { 
    timestamps: { createdAt: 'dateCreation', updatedAt: 'dateModification' } 
    });

adminSchema.pre("save", async function (next) {
    const salt = await bcrypt.genSalt();
    this.motdepasse = await bcrypt.hash(this.motdepasse, salt);
});

adminSchema.statics.login = async function (email, motdepasse){
    const admin = await this.findOne({ email });
    if (admin){
        const auth = await bcrypt.compare(motdepasse, admin.motdepasse);
        if (auth){
            return admin; 
        }
        throw Error("Mot de passe incorrect");

    }
    throw Error("Email incorrect");
}

const Admin = mongoose.model('Admin', adminSchema)
module.exports = Admin

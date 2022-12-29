const Admin = require('../Database/Admin')
const bcrypt = require('bcryptjs')
const jwt = require('jsonwebtoken')

const maxAge = 3*24*60*60;

const createToken = (id) =>{
	return jwt.sign({id},"SRVEDCA-Projet-Electronique",{
		expiresIn: maxAge,
	});
};


const handleErrors = (err) => {
	let errors = { email: "", motdepasse: ""};

	if (err.message === "Email incorrect")
		errors.email = "Cet email n'est pas enregistré";

	if (err.message === "Mot de passe incorrect")
		errors.motdepasse = "Mot de passe incorrect";


	if(err.code===11000){
		errors.email = "Email existe dejà";
		return errors;
	}

	if(err.message.includes("Admin validation failed")){
		Object.values(err.errors).forEach(({properties}) => {
			errors[properties.path] = properties.message;
		});
	}
	return errors;
};

module.exports.register = async (req, res, next) => {
	try{
		const { nom,email,tel, motdepasse, role} = req.body;
		const admin = await Admin.create({
			nom:nom,
			email:email,
			tel:tel,
			motdepasse:motdepasse,
			role:role
		});
		const token = createToken(admin._id);
		res.cookie("jwt", token, {
			withCredentials: true,
			httpOnly: false,
			maxAge: maxAge*1000,
		});
		res.status(201).json({ admin: admin._id, created:true });
	}catch (err) {
		console.log(err);
		const errors = handleErrors(err);
		res.json({ errors, created: false });
	}
};
module.exports.login = async (req, res, next) => {
	try{
		const { email,motdepasse } = req.body;
		const admin = await Admin.login(email, motdepasse);
		const token = createToken(admin._id);
		res.cookie("jwt", token, {
			withCredentials: true,
			httpOnly: false,
			maxAge: maxAge*1000,
		});
		res.status(200).json({ admin: admin._id, created:true });
	}catch (err) {
		console.log(err);
		const errors = handleErrors(err);
		res.json({ errors, created: false });
	}
};